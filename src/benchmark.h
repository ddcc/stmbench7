#ifndef SB7_BENCHMARK_H_
#define SB7_BENCHMARK_H_

#include <iostream>

#include "thread/pthread_wrap.h"
#include "data_holder.h"
#include "thread/thread_fun.h"
#include "operations/operations.h"
#include "constants.h"


namespace sb7 {
	/**
	 * Class that encapsulates all elements of a benchmark.
	 */
	class Benchmark {
		public:
			Benchmark();

			~Benchmark();

			// this includes initialization of data structure used in the
			// benchmark, not other data
			void init();

			void start();

			void dbg();

			void report(ostream &out = std::cout);

		private:
			int *create_array(int size) const;
			int **create_matrix(int rows, int cols) const;
			void free_matrix(int **matrix, int rows) const;

			// short for getting number of operations
			int getOperationNum() const {
				return operations.size();
			}

			void reportTtcHistograms(ostream &out) const;

			// reporting stats
			void reportStats(ostream &out);

			struct opstats {
				int succ;
				int abort;
				int fail;
				int maxttc;
			};

			opstats calcOpstats(int opind, int thnum);
			int calcMaxTtc(int opind, int thind) const;
			int logTtcIndToTtc(int logTtcInd) const;
			void updateOperationTypeStats(OperationType &optype,
				const opstats &os);

			void printSection(ostream &out, const char *text) const;

		private:
			DataHolder dataHolder;

			Operations operations;

			struct {
				pthread_t tid;
				WorkerThreadData wtdata;
			} threads[MAX_THREADS];

			long elapsedTime;
	};
}

#endif // SB7_BENCHMARK_H_
