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

#include "../../tm/tm_spec.h"

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

	// functions that will be invoked by threads/transactions
	void global_init_mem();
	void init_thread_mem();

	// these are non-transactional alloc and free
	void *malloc(size_t size);
	void free(void *);

	// These are transactional alloc and delete. This means that they
	// might perform some operations on transaction commit or abort
	// too.
	void *tx_malloc(size_t size);
	void tx_free(Alloced *);

	// Functions to invoke on transaction start and end.
	void mem_tx_start();
	void mem_tx_commit();
	void mem_tx_abort();
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


// simple malloc
inline void *sb7::malloc(size_t size) {
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

// simple free
inline void sb7::free(void *ptr) {
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

// transactional malloc
inline void *sb7::tx_malloc(size_t size) {
	return tm_malloc(size);
}

// transactional free
inline void sb7::tx_free(Alloced *ptr) {
	tm_free(ptr);
}

inline void sb7::mem_tx_start() {
}

inline void sb7::mem_tx_commit() {
}

inline void sb7::mem_tx_abort() {
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
