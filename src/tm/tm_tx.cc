#ifndef STM_WLPDSTM
#include <setjmp.h>
#endif

#include "tm_tx.h"
#include "tm_spec.h"
#include "tm_ptr.h"
#include "../common/memory.h"

#ifdef STM_WLPDSTM
void* sb7::run_tx(sb7::tx_fun fun, unsigned int ro_flag, sb7::tx_param param,
		sb7::tx_abort_fun abort_fun, sb7::tx_abort_param abort_param) {

	void *ret = NULL;

	unsigned start_flag = sigsetjmp(*wlpdstm_get_long_jmp_buf(), 0);

	if(start_flag != LONG_JMP_ABORT_FLAG) {
		// count aborts
		if(start_flag == LONG_JMP_RESTART_FLAG) {
			mem_tx_abort();
			obj_log_tx_abort();
		}

		mem_tx_start();
		wlpdstm_start_tx();

		ret = fun(param);
		wlpdstm_commit_tx();

		mem_tx_commit();
		obj_log_tx_commit();
	} else {
		mem_tx_commit();
		obj_log_tx_commit();
	}

	return ret;
}

// PF: START
#elif defined(STM_TINY_STM)
void* sb7::run_tx(sb7::tx_fun fun, unsigned int ro_flag, sb7::tx_param param,
		sb7::tx_abort_fun abort_fun, sb7::tx_abort_param abort_param) {

	void *ret = NULL;

	stm_tx_attr_t _a = {.read_only = !!ro_flag};
	sigjmp_buf *_e = ::stm_start(_a);
	int status = _e ? sigsetjmp(*_e, 0) : 0;

	if(status != 0) {
		if(abort_fun != NULL) {
			abort_fun(abort_param);
		}
		mem_tx_abort();
		obj_log_tx_abort();
	}

	mem_tx_start();

	ret = fun(param);
	::stm_commit();

	mem_tx_commit();
	obj_log_tx_commit();

	return ret;
}
// PF: END

#else
void* sb7::run_tx(sb7::tx_fun fun, unsigned int ro_flag, sb7::tx_param param,
		sb7::tx_abort_fun abort_fun, sb7::tx_abort_param abort_param) {
	TX_DATA;
	volatile bool first = true;
#ifdef MACOS
	sigjmp_buf buf;
#else
	jmp_buf buf;
#endif
	sigsetjmp(buf, 0);

	// this is a small hack to distinguish between an abort
	// and first run
	if(!first) {
		if(abort_fun != NULL) {
			abort_fun(abort_param);
		}

		mem_tx_abort();
		obj_log_tx_abort();
	} else {
		first = false;
	}

	// start a transaction
	mem_tx_start();
	TX_START;

	// invoke function
	void *ret = fun(param);

	// try to commit
	TX_COMMIT;

	// if commit was successful we are here
	// and need to cleanup
	mem_tx_commit();
	obj_log_tx_commit();

	return ret;
}
#endif // STM_WLPDSTM
