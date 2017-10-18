/**
 * @file memory.cc
 * 
 * Definition of memory management scheme used in sb7.
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 * @date 2007-07-11
 */

#include <cassert>

#include "memory_explicit.h"

#ifdef COLLECT_MALLOC_STATS
#include <iomanip>

#include "../../parameters.h"
#include "../../helpers.h"

using namespace std;
#endif

// backing storage for timestamp
volatile sb7::v_timestamp sb7::global_timestamp
		__attribute__ ((aligned(64)));

// backing storage for tls gc_key
pthread_key_t sb7::gc_key;

#ifdef COLLECT_MALLOC_STATS
struct sb7::malloc_stats sb7::malloc_stats_arr[MAX_THREADS];
#endif

void sb7::global_init_mem() {
	// for the time being don't worry about freeing this data
	::pthread_key_create(&gc_key, NULL);
}

void sb7::init_thread_mem() {
	// initialize tls data
	sb7::thread_egc_data *egc = (thread_egc_data *)sb7::malloc(
		sizeof(thread_egc_data));

	// initialize fields
	egc->freed = NULL;
#ifndef MM_TXMM
	egc->free_on_abort = NULL;
#endif

#ifdef MM_TXMM
	egc->cleanup_on_abort = NULL;
#endif

	egc->free_on_commit = NULL;

	egc->parent_egc_data = NULL;

	pthread_setspecific(gc_key, egc);

	// get thread identifier
	unsigned tid = sb7::get_tid();

	assert(tid < MAX_THREADS);

	// update global timestamp length
	if(global_timestamp.len < tid + 1) {
		global_timestamp.len = tid + 1;
	}
}

#ifdef COLLECT_MALLOC_STATS
void sb7::print_malloc_stats(std::ostream &out) {
	struct malloc_stats total_stats;

	// initialize total stats
	for(int i = 0;i < MALLOC_STATS_SIZE_COUNT + 1;i++) {
		total_stats.malloc_size[i] = 0;
	}

	total_stats.alloc_count = 0;
	total_stats.free_count = 0;
	total_stats.malloc_ns = 0;
	total_stats.free_ns = 0;
	total_stats.size = 0;


	// sum everything
	for(int i = 0;i < MAX_THREADS;i++) {
		for(int j = 0;j < MALLOC_STATS_SIZE_COUNT + 1;j++) {
			total_stats.malloc_size[j] +=
				malloc_stats_arr[i].malloc_size[j];
		}

		total_stats.alloc_count += malloc_stats_arr[i].alloc_count;
		total_stats.free_count += malloc_stats_arr[i].free_count;
		total_stats.malloc_ns += malloc_stats_arr[i].malloc_ns;
		total_stats.free_ns += malloc_stats_arr[i].free_ns;
		total_stats.size += malloc_stats_arr[i].size;
	}

	long long total_time_ns = total_stats.malloc_ns + total_stats.free_ns;

	double total_time_s =
		total_time_ns / (double)NANOSECONDS_IN_SECOND;
	double malloc_time_s =
		total_stats.malloc_ns / (double)NANOSECONDS_IN_SECOND;
	double free_time_s =
		total_stats.free_ns / (double)NANOSECONDS_IN_SECOND;

	long size_per_thread =
		total_stats.size / parameters.getThreadNum();

	double total_allocations_per_thread =
		total_stats.alloc_count / (double)parameters.getThreadNum();
	double total_frees_per_thread =
		total_stats.free_count / (double)parameters.getThreadNum();

	out << endl << "Memory management statistics:" << endl;

	out << "Time spent in memory management: "
		<< right << setw(6) << total_time_s << "s "
		<< "(malloc: " << right << setw(6) << malloc_time_s << "s, "
		<< "free: " << right << setw(6) << free_time_s << "s)"
		<< endl;

	out << "Total memory allocated: "
		<< right << setw(6) << memSizeToStr(total_stats.size)
		<< " (" << right << setw(6) << memSizeToStr(size_per_thread)
		<< "/thread)" << endl;

	out << "Total number of allocations: "
		<< right << setw(6) << total_stats.alloc_count
		<< " (" << right << setw(6) << total_allocations_per_thread
		<< "per thread)" << endl;
	out << "Total number of deallocations: "
		<< right << setw(6) << total_stats.free_count
		<< " (" << right << setw(6) << total_frees_per_thread
		<< "per thread)" << endl;

	out << "Allocations size distribution: " << endl;

	for(int i = 0;i < MALLOC_STATS_SIZE_COUNT;i++) {
		out << (i + 1) * MALLOC_STATS_BIN_SIZE << " = "
			<< total_stats.malloc_size[i] << " ("
			<< total_stats.malloc_size[i] / (double)total_stats.alloc_count
			<< ")" << endl;
	}

	out << "Rest = "
		<< total_stats.malloc_size[MALLOC_STATS_SIZE_COUNT] << " ("
		<< total_stats.malloc_size[MALLOC_STATS_SIZE_COUNT]
				/ (double)total_stats.alloc_count
		<< ")" << endl;

	out << sizeof(total_stats) << endl;
	
	long start = get_time_ns();
	long end = get_time_ns();

	out << end - start << endl;

	start = get_time_ns();
	malloc(40);
	end = get_time_ns();

	out << end - start << endl;

	start = get_time_ns();
	end = get_time_ns();
	end = get_time_ns();
	end = get_time_ns();
	end = get_time_ns();

	out << end - start << endl;
}
#endif
