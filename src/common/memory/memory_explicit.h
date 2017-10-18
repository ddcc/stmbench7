/**
 * @file memory.h
 * 
 * Declaration of elements required for thread-local explicit memory
 * management are in this file.
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 * @date 2007-07-11
 */

#ifndef SB7_MEMORY_H_
#define SB7_MEMORY_H_

#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <limits>

#include "../../thread/pthread_wrap.h"
#include "../../constants.h"
#include "../tid.h"

#ifdef COLLECT_MALLOC_STATS
#include <iostream>
#include "../../constants.h"
#include "../time.h"
#endif

#ifdef MM_TBB_MALLOC
#include "scalable_allocator.h"
#elif defined MM_TXMM_PLAIN
#include "scalable_allocator.h"
#elif defined MM_TXMM
#include "scalable_allocator.h"
#elif defined MM_HOARD
#include "mm_hoard.h"
#elif defined MM_LEA
#include "mm_lea.h"
#endif

#define MAX_MEM_PTR 128

namespace sb7 {

	class Alloced {
		public:
			virtual ~Alloced() { }
	};

	// These functions are used as a wrapper for real allocate and
	// free functions. This makes it possible to change them easily if
	// needed. For now, they will be just simple malloc and free.
	void *alloc_mem_block(size_t size);
	void free_mem_block(void *mb_ptr);

	// Vector timestamp used for timestamping deleted data.
	struct v_timestamp {
		// avoid false sharing
		unsigned long ts[CACHE_LINE_SIZE * MAX_THREADS];
		unsigned len;
	};

	// Some util functions to use with v_timestamp. These would be a
	// part of timestamp class in a more OO design.
	void copy_v_timestamp(v_timestamp *dest,
		const v_timestamp *src);
	bool is_timestamp_dominated(const v_timestamp *older,
		const v_timestamp *newer);

	// Linked list of memory pointers for deletion.
	struct mem_ptr_arr {
		Alloced *mem_ptr[MAX_MEM_PTR];

		unsigned len;

		v_timestamp vts;

		mem_ptr_arr *next;
	};

	// Some functions that operate on mem_ptr_attr.
	mem_ptr_arr *create_mem_ptr_arr(mem_ptr_arr *next = NULL);
	void free_mem_ptr_arr(mem_ptr_arr *arr);
	void add_ptr_to_arr(mem_ptr_arr **arr, Alloced *ptr,
		v_timestamp *ts = NULL);
	void free_mem_ptr_arr_list(mem_ptr_arr *arr);

	// Every thread holds a list of objects that are to be deleted.
	struct thread_egc_data {
		mem_ptr_arr *freed;

		// these are temporary
#ifndef MM_TXMM
		mem_ptr_arr *free_on_abort;
#endif
		mem_ptr_arr *free_on_commit;

		// finalization functions
		// these are necessary for the OO on top of basic memory
		// manager
		// TODO: use a more general approach for this
#ifdef MM_TXMM
		mem_ptr_arr *cleanup_on_abort;
#endif

		// for nested parallelism
		struct thread_egc_data *parent_egc_data;
	};

	// functions that will be invoked by threads/transactions
	void global_init_mem();
	void init_thread_mem();

	// these are non-transactional alloc and free
	void *malloc(size_t size);
	void free(void *);

	thread_egc_data *get_egc_data();

	// Free something on commit. Only elements that are shared should
	// be freed on commit.
	void tx_free_on_commit(Alloced *);

	// Free something on abort. Only temporary data, that can become
	// permanent should be freed on abort.	
#ifndef MM_TXMM
	void tx_free_on_abort(Alloced *);
#endif

#ifdef MM_TXMM
	void tx_cleanup_on_abort(Alloced *);
#endif


	// These are transactional alloc and delete. This means that they
	// might perform some operations on transaction commit or abort
	// too.
	void *tx_malloc(size_t size);
	void tx_free(Alloced *);

	// Functions to invoke on transaction start and end.
	void mem_tx_start();
	void mem_tx_start(struct thread_egc_data *pegc);
	void mem_tx_commit();
	void mem_tx_abort();

	void free_dominated();

	//////////////////////////////
	// Declare some data I need //
	//////////////////////////////

	// This is a global timestamp whose components are incremented by
	// different threads.
	extern volatile v_timestamp global_timestamp;

	extern ::pthread_key_t gc_key;


	//////////////////
	// malloc stats //
	//////////////////

#ifdef COLLECT_MALLOC_STATS
	const static int MALLOC_STATS_SIZE_COUNT = 16;
	const static int MALLOC_STATS_BIN_SIZE = 8;

	struct malloc_stats {
		// 8 byte bins
		long long malloc_size[MALLOC_STATS_SIZE_COUNT + 1];
		long long alloc_count;
		long long free_count;
		long long malloc_ns;
		long long free_ns;
		long long size;
		char pad[80];
	};

	long collect_malloc_stats_start(size_t size);
	void collect_malloc_stats_end(size_t size, long start_time);
	long collect_free_stats_start();
	void collect_free_stats_end(long start_time);
	int map_size_to_bin(size_t size);
	void print_malloc_stats(std::ostream &out);

	extern struct malloc_stats malloc_stats_arr[];
#endif
}

// I split declarations and definitions, but want them inline, so they
// are in the header file.

////////////////////////////////
// malloc/free implementation //
////////////////////////////////

inline void *sb7::alloc_mem_block(size_t size) {
#ifdef COLLECT_MALLOC_STATS
	long stime = collect_malloc_stats_start(size);
#endif

#ifdef MM_TBB_MALLOC
	void *ret = scalable_malloc(size);
#elif defined MM_TXMM_PLAIN
	void *ret = scalable_malloc(size);
#elif defined MM_TXMM
	/* use non-tx functions */
	void *ret = scalable_malloc(size);
#elif defined MM_HOARD
	void *ret = hoard_malloc(size);
#elif defined MM_LEA
	void *ret = lea_malloc(size);
#else
	void *ret = ::malloc(size);
#endif

	assert(ret != NULL);

#ifdef COLLECT_MALLOC_STATS
	collect_malloc_stats_end(size, stime);
#endif
	return ret;
}

inline void sb7::free_mem_block(void *ptr) {
#ifdef COLLECT_MALLOC_STATS
	long stime = collect_free_stats_start();
#endif

#ifdef MM_TBB_MALLOC
	scalable_free(ptr);
#elif defined MM_TXMM_PLAIN
	scalable_free(ptr);
#elif defined MM_TXMM
	/* use non-tx functions */
	scalable_free(ptr);
#elif defined MM_HOARD
	hoard_free(ptr);
#elif defined MM_LEA
	lea_free(ptr);
#else
	::free(ptr);
#endif

#ifdef COLLECT_MALLOC_STATS
	collect_free_stats_end(stime);
#endif
}

//////////////////////////
// timestamp operations //
//////////////////////////

inline void sb7::copy_v_timestamp(sb7::v_timestamp *dest,
		const sb7::v_timestamp *src) {
	unsigned len = src->len;
	dest->len = len;

	for(unsigned i = 0;i < len;i++) {
		dest->ts[i * CACHE_LINE_SIZE] = src->ts[i * CACHE_LINE_SIZE];
	}
}

// For one timestamp to dominate another one it is necessary that all
// transactions that were ongoing in the older one have finished in
// the newer one. Thread is executing a transaction if it's timestamp
// components is odd. 
//inline bool sb7::is_timestamp_dominated(const sb7::v_timestamp *older,
		//const sb7::v_timestamp *newer) {
	//unsigned len = older->len;
	//bool ret = true;

	//for(unsigned i = 0;i < len;i++) {
		//unsigned ots = older->ts[i * CACHE_LINE_SIZE];
		//unsigned nts = newer->ts[i * CACHE_LINE_SIZE];

		//if(!(nts > ots || nts % 2 == 0)) {
			//ret = false;
			//break;
		//}
	//}

	//return ret;
//}

inline bool sb7::is_timestamp_dominated(const sb7::v_timestamp *older,
		const sb7::v_timestamp *newer) {
	unsigned len = older->len;
	bool ret = true;

	for(unsigned i = 0;i < len;i++) {
		unsigned ots = older->ts[i * CACHE_LINE_SIZE];
		unsigned nts = newer->ts[i * CACHE_LINE_SIZE];

		if(!(nts > ots || nts % 2 == 0)) {
			ret = false;
			break;
		}
	}

	return ret;
}

inline sb7::mem_ptr_arr *sb7::create_mem_ptr_arr(
		sb7::mem_ptr_arr *next) {
	mem_ptr_arr *ret = (mem_ptr_arr *)alloc_mem_block(
		sizeof(mem_ptr_arr));
	ret->len = 0;
	ret->next = next;
	return ret;
}

// simply deallocate memory
inline void sb7::free_mem_ptr_arr(sb7::mem_ptr_arr *arr) {
	sb7::free_mem_block(arr);
}

// one function to call when adding pointers to array of pointers
inline void sb7::add_ptr_to_arr(sb7::mem_ptr_arr **arr,
		sb7::Alloced *ptr, sb7::v_timestamp *ts) {
	sb7::mem_ptr_arr *curr = *arr;

	// if mem_ptr is full, create a new one
	if(curr == NULL) {
		curr = sb7::create_mem_ptr_arr();
		*arr = curr;
	}

	curr->mem_ptr[curr->len] = ptr;
	curr->len++;

	// make sure not to leave empty first mem_ptr_arr
	if(curr->len == MAX_MEM_PTR) {
		if(ts != NULL) {
			copy_v_timestamp(&(curr->vts), ts);
		}

		*arr = sb7::create_mem_ptr_arr(curr);
	}
}

inline void sb7::free_mem_ptr_arr_list(sb7::mem_ptr_arr *arr) {
	sb7::mem_ptr_arr *curr = arr;
	sb7::mem_ptr_arr *prev;

	while(curr != NULL) {
		prev = curr;
		curr = curr->next;
		sb7::free_mem_ptr_arr(prev);
	}
}

// simple malloc
inline void *sb7::malloc(size_t size) {
	return sb7::alloc_mem_block(size);
}

// simple free
inline void sb7::free(void *ptr) {
	sb7::free_mem_block(ptr);
}

// get relevant tls data
inline sb7::thread_egc_data *sb7::get_egc_data() {
	return (sb7::thread_egc_data *)::pthread_getspecific(sb7::gc_key);
}

// prepate something to delete if transaction commits
inline void sb7::tx_free_on_commit(Alloced *ptr) {
	sb7::thread_egc_data *egc = get_egc_data();
	sb7::add_ptr_to_arr(&(egc->free_on_commit), ptr);
}

#ifndef MM_TXMM
// prepate something to delete if transaction aborts
inline void sb7::tx_free_on_abort(Alloced *ptr) {
	sb7::thread_egc_data *egc = get_egc_data();
	sb7::add_ptr_to_arr(&(egc->free_on_abort), ptr);
}
#endif

#ifdef MM_TXMM
// prepate something to delete if transaction aborts
inline void sb7::tx_cleanup_on_abort(Alloced *ptr) {
	sb7::thread_egc_data *egc = get_egc_data();
	sb7::add_ptr_to_arr(&(egc->cleanup_on_abort), ptr);
}
#endif

// transactional malloc
inline void *sb7::tx_malloc(size_t size) {
#ifdef MM_TXMM
	void *ptr = scalable_tx_malloc(size);
	sb7::tx_cleanup_on_abort((Alloced *)ptr);
	return ptr;
#else
	void *ptr = sb7::alloc_mem_block(size);
	sb7::tx_free_on_abort((Alloced *)ptr);
	return ptr;
#endif
}

// transactional free
inline void sb7::tx_free(Alloced *ptr) {
	sb7::tx_free_on_commit(ptr);
}

inline void sb7::mem_tx_start() {
	mem_tx_start(NULL);
}

inline void sb7::mem_tx_start(struct thread_egc_data *pegc) {
	global_timestamp.ts[get_tid() * CACHE_LINE_SIZE]++;

	if(pegc != NULL) {
		sb7::thread_egc_data *egc = get_egc_data();
		egc->parent_egc_data = pegc;
	}

#ifdef MM_TXMM
	txmm_start_tx();
#endif
}

inline void sb7::mem_tx_commit() {
	// increase global counter
	global_timestamp.ts[get_tid() * CACHE_LINE_SIZE]++;

	// get gc data to perform cleanup
	sb7::thread_egc_data *egc = sb7::get_egc_data();

	if(egc->parent_egc_data != NULL) {
		struct thread_egc_data *parent_egc = egc->parent_egc_data;

		// merge free on commit
		sb7::mem_ptr_arr *curr = egc->free_on_commit;
		sb7::mem_ptr_arr *prev;
		
		while(curr != NULL) {
			unsigned len = curr->len;
			
			for(unsigned i = 0;i < len;i++) {
				sb7::add_ptr_to_arr(&(parent_egc->free_on_commit), curr->mem_ptr[i]);
			}
			
			prev = curr;
			curr = curr->next;
			sb7::free_mem_ptr_arr(prev);
		}
		
		egc->free_on_commit = NULL;

		// merge free on abort
		curr = egc->free_on_abort;
		
		while(curr != NULL) {
			unsigned len = curr->len;
			
			for(unsigned i = 0;i < len;i++) {
				sb7::add_ptr_to_arr(&(parent_egc->free_on_abort), curr->mem_ptr[i]);
			}
			
			prev = curr;
			curr = curr->next;
			sb7::free_mem_ptr_arr(prev);
		}
		
		egc->free_on_abort = NULL;
	} else {
#ifdef MM_TXMM
		txmm_commit_tx();
		
		sb7::free_mem_ptr_arr_list(egc->cleanup_on_abort);
		egc->cleanup_on_abort = NULL;
#endif
		
#ifndef MM_TXMM
		// first free list of objects to delete on abort
		sb7::free_mem_ptr_arr_list(egc->free_on_abort);
		egc->free_on_abort = NULL;
#endif

		// now put all of the pointers into freed list
		sb7::mem_ptr_arr *curr = egc->free_on_commit;
		sb7::mem_ptr_arr *prev;

		while(curr != NULL) {
			unsigned len = curr->len;

			for(unsigned i = 0;i < len;i++) {
				sb7::add_ptr_to_arr(&(egc->freed), curr->mem_ptr[i],
					(v_timestamp *)&sb7::global_timestamp);
			}
			
			prev = curr;
			curr = curr->next;
			sb7::free_mem_ptr_arr(prev);
		}

		egc->free_on_commit = NULL;

		// try to actually release some memory
		sb7::free_dominated();
	}
}

// look for something that can be safely released
inline void sb7::free_dominated() {
	// get gc data to perform cleanup
	sb7::thread_egc_data *egc = sb7::get_egc_data();
	sb7::mem_ptr_arr *curr = egc->freed;
	sb7::mem_ptr_arr *prev = curr;

	// skip the first as it is never full
	if(curr != NULL) {
		curr = curr->next;
	}

	// now search for the first that can be deleted
	while(curr != NULL) {
		if(sb7::is_timestamp_dominated(&(curr->vts), 
				(const sb7::v_timestamp *)&global_timestamp)) {
			break;
		}

		prev = curr;
		curr = curr->next;
	}

	// cut off all dominated arrays, if any
	if(curr != NULL) {
		prev->next = NULL;
	}

	// curr now points to the first one that can be deleted
	while(curr != NULL) {
		unsigned len = curr->len;

		for(unsigned i = 0;i < len;i++) {
			delete(curr->mem_ptr[i]);
		}
		
		prev = curr;
		curr = curr->next;
		sb7::free_mem_ptr_arr(prev);
	}
}

inline void sb7::mem_tx_abort() {
	global_timestamp.ts[get_tid() * CACHE_LINE_SIZE]++;

	// get gc data to perform cleanup
	sb7::thread_egc_data *egc = sb7::get_egc_data();

	// first free list of objects to delete on commit
	sb7::free_mem_ptr_arr_list(egc->free_on_commit);
	egc->free_on_commit = NULL;

#ifdef MM_TXMM
	// TODO: think about improving performance here by using bulk
	// aborts in some way
	// One idea is to mark the beginning of this cleanup phase and
	// to use this to perform bulk deletes.
	// Another option would be to record even non-tx allocations during
	// the transaction and to use this to perform fast rollbacks. I am
	// sceptical about this second solution as it does not seem easy
	// to implement.

	// cleanup all objects that should be deleted on abort
	sb7::mem_ptr_arr *curr = egc->cleanup_on_abort;
	sb7::mem_ptr_arr *prev;

	while(curr != NULL) {
		unsigned len = curr->len;

		for(unsigned i = 0;i < len;i++) {
			curr->mem_ptr[i]->~Alloced();
		}
		
		prev = curr;
		curr = curr->next;
		sb7::free_mem_ptr_arr(prev);
	}

	egc->cleanup_on_abort = NULL;

	// now invoke the tx abort
	// this must come after invoking destructors, as after abort some
	// of the memory might be given to other threads
	txmm_abort_tx();
#else
	// delete all objects that should be deleted on abort
	sb7::mem_ptr_arr *curr = egc->free_on_abort;
	sb7::mem_ptr_arr *prev;

	while(curr != NULL) {
		unsigned len = curr->len;

		for(unsigned i = 0;i < len;i++) {
			delete(curr->mem_ptr[i]);
		}
		
		prev = curr;
		curr = curr->next;
		sb7::free_mem_ptr_arr(prev);
	}

	egc->free_on_abort = NULL;
#endif

	// run free_dominated here to release memory asap
	//sb7::free_dominated();
}

// Allocator to use with STL containers.
namespace sb7 {
	template <typename T>
	class Sb7Allocator {
		public:
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;

			template <class U>
			struct rebind {
				typedef Sb7Allocator<U> other;
			};

			pointer address(reference value) const {
				return &value;
			}

			const_pointer address(const_reference value) const {
				return &value;
			}

			Sb7Allocator() { }

			Sb7Allocator(const Sb7Allocator&) { }

			template <class U>
			Sb7Allocator(const Sb7Allocator<U>&) { }

			~Sb7Allocator() { }

			pointer allocate(size_type num, const void* = 0) {
				return static_cast<pointer>(
					sb7::malloc(num * sizeof(T)));
			}

			void construct(pointer p, const T& value) {
				new((void*)p)T(value);
			}

			void destroy(pointer p) {
				p->~T();
			}

			void deallocate(pointer p, size_type num) {
				sb7::free(static_cast<void*>(p));
			}

        inline size_type max_size() const { 
        return std::numeric_limits<size_type>::max() / sizeof(T);
	}
	};

    template <typename T1, typename T2>
    bool operator==(const Sb7Allocator<T1>&, const Sb7Allocator<T2>&) {
        return true;
    }

    template <typename T1, typename T2>
    bool operator!=(const Sb7Allocator<T1>&, const Sb7Allocator<T2>&) {
        return false;
    }
}

// Convenience class to inherit from when creating transactional
// objects.
namespace sb7 {

	class Sb7TxAlloced : public Alloced {
		public:
			void* operator new(size_t size) {
				return tx_malloc(size);
			}

			// don't do tx_free, as that will lead to infinite looping
			void operator delete(void* ptr) {
				sb7::free(ptr);
			}

			virtual ~Sb7TxAlloced() { }
	};

	class Sb7Alloced : public Alloced {
		public:
			void* operator new(size_t size) {
				return sb7::malloc(size);
			}

			void operator delete(void* ptr) {
				sb7::free(ptr);
			}

			virtual ~Sb7Alloced() { }
	};
}

#ifdef COLLECT_MALLOC_STATS
inline long sb7::collect_malloc_stats_start(size_t size) {
	int tid = get_tid();

	if(tid == 0) {
		return 0;
	}

	malloc_stats_arr[tid].alloc_count++;
	malloc_stats_arr[tid].malloc_size[map_size_to_bin(size)]++;
	malloc_stats_arr[tid].size += size;
	return get_time_ns();
}

inline void sb7::collect_malloc_stats_end(size_t size, long stime) {
	int tid = get_tid();

	if(tid == 0) {
		return;
	}

	long etime = get_time_ns();
	malloc_stats_arr[tid].malloc_ns += etime - stime;
}

inline long sb7::collect_free_stats_start() {
	int tid = get_tid();

	if(tid == 0) {
		return 0;
	}

	malloc_stats_arr[tid].free_count++;
	return get_time_ns();
}

inline void sb7::collect_free_stats_end(long stime) {
	int tid = get_tid();

	if(tid == 0) {
		return;
	}

	long etime = get_time_ns();
	malloc_stats_arr[tid].free_ns += etime - stime;
}

inline int sb7::map_size_to_bin(size_t size) {
	int ret = size;
	ret /= MALLOC_STATS_BIN_SIZE;

	if(ret >= MALLOC_STATS_SIZE_COUNT) {
		ret = MALLOC_STATS_SIZE_COUNT;
	} else {
		ret = size;
	}

	return ret;
}

#endif

#endif
