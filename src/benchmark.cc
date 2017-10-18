#include <cstdlib>
#include <cmath>
#include <iomanip>

#ifdef COLLECT_TXMM_STATISTICS
#include <cstdio>
#endif

#include "common/time.h"
#include "benchmark.h"
#include "parameters.h"
#include "helpers.h"
#include "thread/thread.h"
#include "common/memory.h"

using namespace std;

sb7::Benchmark::Benchmark() : operations(&dataHolder) {
	// initialize thread local data
	global_thread_init();

	// initialize threads array
	int threadNum = parameters.getThreadNum();
	int operationNum = getOperationNum();
	int max_low_ttc = parameters.getMaxLowTtc();
	int high_ttc_entries = parameters.getHighTtcEntries();

	// precalculate logs
	double max_low_ttc_log = ::log(max_low_ttc + 1);
	double high_ttc_log_base = ::log(parameters.getHighTtcLogBase());

	for(int i = 0;i < threadNum;i++) {
		threads[i].wtdata.stopped = false;
		threads[i].wtdata.operations = &operations;

		threads[i].wtdata.opnum = operationNum;
		threads[i].wtdata.max_low_ttc = max_low_ttc;
		threads[i].wtdata.high_ttc_entries = high_ttc_entries;

		threads[i].wtdata.successful_ops = create_array(operationNum);
		threads[i].wtdata.aborted_ops = create_array(operationNum);
		threads[i].wtdata.failed_ops = create_array(operationNum);
		threads[i].wtdata.operations_ttc = create_matrix(
			operationNum, max_low_ttc + 1);
		threads[i].wtdata.operations_high_ttc_log = create_matrix(
			operationNum, high_ttc_entries);

		threads[i].wtdata.max_low_ttc_log = max_low_ttc_log;
		threads[i].wtdata.high_ttc_log_base = high_ttc_log_base;
	}

	elapsedTime = -1;
}

int *sb7::Benchmark::create_array(int size) const {
	int *array = new int[size];

	for(int i = 0;i < size;i++) {
		array[i] = 0;
	}

	return array;
}

int **sb7::Benchmark::create_matrix(int rows, int cols) const {
	int **matrix = new int*[rows];

	for(int i = 0;i < rows;i++) {
		matrix[i] = create_array(cols);
	}

	return matrix;
}

sb7::Benchmark::~Benchmark() {
	int threadNum = parameters.getThreadNum();
	int operationNum = getOperationNum();

	for(int i = 0;i < threadNum;i++) {
		delete [] threads[i].wtdata.successful_ops;
		delete [] threads[i].wtdata.aborted_ops;
		delete [] threads[i].wtdata.failed_ops;
		free_matrix(threads[i].wtdata.operations_ttc, operationNum);
		free_matrix(threads[i].wtdata.operations_high_ttc_log, operationNum);
	} 
}

void sb7::Benchmark::free_matrix(int **matrix, int rows) const {
	for(int i = 0;i < rows;i++) {
		int *row = matrix[i];
		delete [] row;
	}

	delete [] matrix;
}

void sb7::Benchmark::init() {
	try {
		// initialize structure of data holder
		sb7::init_data_holder(&dataHolder);
	} catch (Sb7Exception exc) {
		cerr << exc.getMsg() << endl;
		::exit(1);
	}
}

void sb7::Benchmark::start() {
	long start_time = get_time_ms();

	// create and run threads
	for(int i = 0;i < parameters.getThreadNum();i++) {
		// initialize worker thread data
		threads[i].wtdata.stopped = false;

		// TODO catch errors
		pthread_create(&(threads[i].tid), NULL, worker_thread,
			&(threads[i].wtdata));
	}

	// wait for experiment to finish
	sleep(parameters.getExperimentLengthMs());

	// signal all threads to stop
	for(int i = 0;i < parameters.getThreadNum();i++) {
		threads[i].wtdata.stopped = true;
	}

	// wait for all threads to stop
	for(int i = 0;i < parameters.getThreadNum();i++) {
		// TODO catch errors
		pthread_join(threads[i].tid, NULL);
	}

	long end_time = get_time_ms();
	elapsedTime = end_time - start_time;
}

void sb7::Benchmark::report(ostream &out) {
	if(parameters.shouldReportTtcHistograms()) {
		reportTtcHistograms(out);
	}

	reportStats(out);
}

void sb7::Benchmark::reportTtcHistograms(ostream &out) const {
	// TODO
}

void sb7::Benchmark::reportStats(ostream &out) {
	std::vector<Operation *> ops = operations.getOperations();
	std::vector<OperationType> &optypes = operations.getOperationTypes();
	int threadNum = parameters.getThreadNum();
	int operationTypesNum = optypes.size();
	int operationNum = getOperationNum();

	// print operation statistics
	sb7::printSection(out, "Detailed results");

	for(int i = 0;i < operationNum;i++) {
		opstats os = calcOpstats(i, threadNum);
		updateOperationTypeStats(optypes[ops[i]->type], os);

		out << "Opearation " << setw(4) << right << ops[i]->name << ":  "
			<< " successful = " << left << setw(6) << os.succ
			<< " aborts = " << left << setw(6) << os.abort
			<< " maxttc = " << left << setw(6) << os.maxttc
			<< " failed = " << left << setw(6) << os.fail
			<< endl;
	}

	out << endl;

	// print sample errors
	sb7::printSection(out, "Sample errors (operation ratios [%])");

	// calc total success and failure
	int totalSuccess = 0;
	int totalAborted = 0;
	int totalFailure = 0;

	for(int i = 0;i < operationTypesNum;i++) {
		totalSuccess += optypes[i].success;
		totalAborted += optypes[i].aborted;
		totalFailure += optypes[i].failure;
	}

	// report errors per type
	double totalError = 0.0;
	double totalTError = 0.0;

	for(int i = 0;i < operationTypesNum;i++) {
		OperationType &optype = optypes[i];
		double expectedRatio = optype.probability * optype.count * 100;
		double realRatio = (double)optype.success / totalSuccess * 100;
		double error = ::fabs(realRatio - expectedRatio);
		double tRealRatio = (double)(optype.success + optype.failure) /
			(double)(totalSuccess + totalFailure) * 100;
		double tError = ::fabs(tRealRatio - expectedRatio);

		totalError += error;
		totalTError += tError;

		out << right << setw(23) << optype.name << ":  "
			<< fixed << setprecision(2)
			<< "expected = " << right << setw(6) << expectedRatio
			<< "  successful = " << right << setw(6) << realRatio
			<< "  error = " << right << setw(6) << error << "  "
			<< "  (total = " << right << setw(6) << tRealRatio
			<< "  error = " << right << setw(6) << tError << ")"
			<< endl;
	}

	out << endl;

	// report summary results
	sb7::printSection(out, "Summary results");

	int total = totalSuccess + totalFailure;

	for(int i = 0;i < operationTypesNum;i++) {
		OperationType &optype = optypes[i];
		int totalTypeOps = optype.success + optype.failure;

		out << right << setw(23) << optype.name << ":  "
			<< setprecision(2)
			<< "successful = " << right << setw(6) << optype.success
			<< "  aborts = " << right << setw(6) << optype.aborted
			<< "  maxttc = " << right << setw(6) << optype.maxttc
			<< "  failed = " << right << setw(6) << optype.failure
			<< "  total = " << right << setw(6) << totalTypeOps
			<< endl;
	}

	out << endl;

	// report total statistics
	out << "Total sampleError: " << totalError << "%"
		<< "  (" << totalTError << "% including failed)"
		<< endl;

	// TODO use constant for 1000 - ms in s
	//double totalThroughput = (double)totalSuccess / elapsedTime * 1000;
	//double totalTThroughput = (double)total / elapsedTime * 1000;
	double totalThroughput = (double)totalSuccess / parameters.getExperimentLengthMs() * 1000;
	double totalTThroughput = (double)total / parameters.getExperimentLengthMs() * 1000;

	out << "Total throughput: " << totalThroughput << " op/s"
		<< "  (" << totalTThroughput << " op/s including failed)"
		<< endl;

	double totalAbortedSec = (double)totalAborted / elapsedTime * 1000;

	out << "Total aborts: " << totalAbortedSec << " op/s"
		<< endl;

	out << "Elapsed time: " << elapsedTime / 1000.0 << " s" << endl;

#ifdef COLLECT_MALLOC_STATS
	print_malloc_stats(out);
#endif
#ifdef MM_TXMM
	::txmm_stats_print(stdout);
#endif

	out << endl;
}

sb7::Benchmark::opstats sb7::Benchmark::calcOpstats(int opind, int thnum) {
	int succ = 0, aborted = 0, fail = 0, maxttc = 0;

	for(int i = 0;i < thnum;i++) {
		succ += threads[i].wtdata.successful_ops[opind];
		aborted += threads[i].wtdata.aborted_ops[opind];
		fail += threads[i].wtdata.failed_ops[opind];
		int currThMaxTtc = calcMaxTtc(opind, i);
		maxttc = MAX(maxttc, currThMaxTtc);
	}

	opstats ret = { succ, aborted, fail, maxttc };
	return ret;
}

int sb7::Benchmark::calcMaxTtc(int opind, int thind) const {
	for(int i = parameters.getHighTtcEntries() - 1;i >= 0;i--) {
		if(threads[thind].wtdata.operations_high_ttc_log[opind][i] > 0) {
			return logTtcIndToTtc(i);
		}
	}

	for(int i = parameters.getMaxLowTtc();i >= 0;i--) {
		if(threads[thind].wtdata.operations_ttc[opind][i] > 0) {
			return i;
		}
	}

	return 0;
}

int sb7::Benchmark::logTtcIndToTtc(int logTtcInd) const {
	return (int)((parameters.getMaxLowTtc() + 1) *
		pow(parameters.getHighTtcLogBase(), logTtcInd));
}

void sb7::Benchmark::updateOperationTypeStats(OperationType &optype,
		const opstats &os) {
	optype.success += os.succ;
	optype.aborted += os.abort;
	optype.failure += os.fail;
	optype.maxttc = MAX(optype.maxttc, os.maxttc);
}

