#include <cmath>
#include <setjmp.h>

#include "pthread_wrap.h"
#include "../tm/tm_spec.h"
#include "../common/time.h"
#include "../common/memory.h"
#include "../tm/tm_tx.h"
#include "thread.h"
#include "thread_fun.h"

#include "../parameters.h"
#include "../data_holder.h"
#include "../sb7_exception.h"
#include "../helpers.h"

namespace sb7 {
	static void *init_single_tx(void *data) {
		DataHolder *dataHolder = (DataHolder *)data;
		dataHolder->init();
		return NULL;
	}
}

void *sb7::init_data_holder(void *data) {
	thread_init();

	if(parameters.shouldInitSingleTx()) {
		run_tx(init_single_tx, 0, data);
	} else {
		DataHolder *dataHolder = (DataHolder *)data;
		dataHolder->initTx();
	}

	// finish up this thread
	thread_clean();

	// just return something
	return NULL;
}

#ifdef STM_WLPDSTM
void *sb7::worker_thread(void *data) {
	thread_init();

	WorkerThreadData *wtdata = (WorkerThreadData *)data;

	while(!wtdata->stopped) {
		int opind = wtdata->getOperationRndInd();
		const Operation *op = wtdata->operations->getOperations()[opind];

		// get start time
		long start_time = get_time_ms();

		// execute transaction
		unsigned start_flag = sigsetjmp(*wlpdstm_get_long_jmp_buf(), 0);

		if(start_flag != LONG_JMP_ABORT_FLAG) {
			// count aborts
			if(start_flag == LONG_JMP_RESTART_FLAG) {
				mem_tx_abort();
				obj_log_tx_abort();

				wtdata->aborted_ops[opind]++;
			}

			mem_tx_start();
			wlpdstm_start_tx();

			try {
				// transaction body
				op->run();
			} catch (Sb7Exception) {
				wlpdstm_abort_tx();
			}

			wlpdstm_commit_tx();

			mem_tx_commit();
			obj_log_tx_commit();
		} else {
			// failed and aborted
			mem_tx_abort();
			obj_log_tx_abort();

			wtdata->failed_ops[opind]++;

			// skip this calculation below
			continue;
		}

		// get end time
		long end_time = get_time_ms();

		wtdata->successful_ops[opind]++;
		long ttc = end_time - start_time;

		if(ttc <= wtdata->max_low_ttc) {
			wtdata->operations_ttc[opind][ttc]++;
		} else {
			double logHighTtc = (::log(ttc) - wtdata->max_low_ttc_log) /
				wtdata->high_ttc_log_base;
			int intLogHighTtc =
				MIN((int)logHighTtc, wtdata->high_ttc_entries - 1);
			wtdata->operations_high_ttc_log[opind][intLogHighTtc]++;
		}
	}

	thread_clean();

	// just return something
	return NULL;
}

// PF: START
#elif defined(STM_TINY_STM)
void *sb7::worker_thread(void *data) {
	thread_init();

	WorkerThreadData *wtdata = (WorkerThreadData *)data;
	bool hintRo = parameters.shouldHintRo();

	while(!wtdata->stopped) {
		int opind = wtdata->getOperationRndInd();
		const Operation *op = wtdata->operations->getOperations()[opind];

		// check if operation is read only
		stm_tx_attr_t _a = {.read_only = hintRo && op->isReadOnly()};

		volatile bool abort = false;

		// get start time
		long start_time = get_time_ms();

		// execute transaction
		sigjmp_buf *_e = ::stm_start(_a);
		int status = _e ? sigsetjmp(*_e, 0) : 0;

		if(!abort) {
			// count aborts
			if(status != 0) {
				mem_tx_abort();
				obj_log_tx_abort();

				wtdata->aborted_ops[opind]++;
			}

			mem_tx_start();

			try {
				// transaction body
				op->run();
			} catch (Sb7Exception) {
				abort = true;
				::stm_stop(0);
				goto fail;
			}

			::stm_commit();

			mem_tx_commit();
			obj_log_tx_commit();
		} else {
fail:
			// failed and aborted
			mem_tx_abort();
			obj_log_tx_abort();

			wtdata->failed_ops[opind]++;

			// skip this calculation below
			continue;
		}

		// get end time
		long end_time = get_time_ms();

		wtdata->successful_ops[opind]++;
		long ttc = end_time - start_time;

		if(ttc <= wtdata->max_low_ttc) {
			wtdata->operations_ttc[opind][ttc]++;
		} else {
			double logHighTtc = (::log(ttc) - wtdata->max_low_ttc_log) /
				wtdata->high_ttc_log_base;
			int intLogHighTtc =
				MIN((int)logHighTtc, wtdata->high_ttc_entries - 1);
			wtdata->operations_high_ttc_log[opind][intLogHighTtc]++;
		}
	}

#ifdef STM_TINY_STM_DBG
	int nb_aborts;
	stm_get_parameter(tx, "nb_aborts", &nb_aborts);

	if(nb_aborts != 0) {
		std::cout << "Transaction aborted " << nb_aborts
			<< " times" << std::endl;
	}
#endif

	thread_clean();

	// just return something
	return NULL;
}
// PF: END

#else
void *sb7::worker_thread(void *data) {
	thread_init();

	TX_DATA;
	WorkerThreadData *wtdata = (WorkerThreadData *)data;
	bool hintRo = parameters.shouldHintRo();

	while(!wtdata->stopped) {
		int opind = wtdata->getOperationRndInd();
		const Operation *op = wtdata->operations->getOperations()[opind];

		// check if operation is read only
		int ro_flag = (hintRo && op->isReadOnly());
#ifdef STM_TL2
		volatile bool failed = false;
#endif
		volatile bool abort = false;

		try {
			// get start time
			long start_time = get_time_ms();

			//////////////////////
			// start of tx code //
			//////////////////////

			volatile bool first = true;
#ifdef MACOS
			sigjmp_buf buf;
#else
			jmp_buf buf;
#endif
			sigsetjmp(buf, 1);

#ifdef STM_TL2
			// deal with failed transactions in this manner
			if(failed) {
				failed = false;

				// perform abort cleanup
				mem_tx_abort();
				obj_log_tx_abort();
				continue;
			}
#endif

			// this is a small hack to distinguish between an abort
			// and first run
			if(!first) {
				wtdata->aborted_ops[opind]++;

				// perform abort cleanup
				mem_tx_abort();
				obj_log_tx_abort();
			} else {
				first = false;
			}

			// start a transaction
			mem_tx_start();
			TX_START;

			// perform actual operation
			op->run();

			// try to comit
			TX_COMMIT;

			// if commit was successful we are here
			// and need to cleanup
			mem_tx_commit();
			obj_log_tx_commit();

			////////////////////
			// end of tx code //
			////////////////////

			// get end time
			long end_time = get_time_ms();

			wtdata->successful_ops[opind]++;
			int ttc = (int)(end_time - start_time);

			if(ttc <= wtdata->max_low_ttc) {
				wtdata->operations_ttc[opind][ttc]++;
			} else {
				double logHighTtc = (::log(ttc) - wtdata->max_low_ttc_log) /
					wtdata->high_ttc_log_base;
				int intLogHighTtc =
					MIN((int)logHighTtc, wtdata->high_ttc_entries - 1);
				wtdata->operations_high_ttc_log[opind][intLogHighTtc]++;
			}
		} catch (Sb7Exception) {
			wtdata->failed_ops[opind]++;
#ifdef STM_TL2
			failed = true;
#endif
			abort = true;
		}

		// do it like this in order to free exception memory
		if(abort) {
			abort = false;
			TX_ABORT;
#ifdef STM_TINY_STM
			mem_tx_abort();
			obj_log_tx_abort();
#endif
		}
	}

#ifdef STM_TINY_STM_DBG
	int nb_aborts;
	stm_get_parameter(tx, "nb_aborts", &nb_aborts);

	if(nb_aborts != 0) {
		std::cout << "Transaction aborted " << nb_aborts
			<< " times" << std::endl;
	}
#endif

	thread_clean();

	// just return something
	return NULL;
}
#endif // STM_WLPDSTM

int sb7::WorkerThreadData::getOperationRndInd() const {
	double oprnd = get_random()->nextDouble();
	const std::vector<double> &opRat = operations->getOperationCdf();
	int opind = 0;

	while(opRat[opind] < oprnd) {
		opind++;
	}

	return opind;
}



