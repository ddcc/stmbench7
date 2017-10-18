#ifndef SB7_THREAD_FUN_H_
#define SB7_THREAD_FUN_H_

#include "../data_holder.h"
#include "../operations/operations.h"

namespace sb7 {

	void *init_data_holder(void *);

	/**
	 * Structure that contains all data needed by worker threads
	 * for executing operations and reporting results of executions.
	 */
	struct WorkerThreadData {
		// flag specifying that thread should stop executing
		volatile bool stopped;

		// operations that are being performed
		Operations *operations;

		// sizes of reporting arrays
		int opnum;
		int max_low_ttc;
		int high_ttc_entries;

		// arrays with reporting data
		int *successful_ops;
		int *aborted_ops;
		int *failed_ops;
		int **operations_ttc;
		int **operations_high_ttc_log;

		// precomputed logs
		double max_low_ttc_log;
		double high_ttc_log_base;

		// some functions that can help
		int getOperationRndInd() const;
	};

	void *worker_thread(void *);
}

#endif
