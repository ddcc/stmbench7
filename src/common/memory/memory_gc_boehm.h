#ifndef SB7_MEMORY_H_
#define SB7_MEMORY_H_

#include <cstddef>

#define GC_THREADS
#define GC_REDIRECT_TO_LOCAL
#include "gc.h"

namespace sb7 {

	class Alloced {
		public:
			virtual ~Alloced() { }
	};

	void global_init_mem();
	void init_thread_mem();

	// These are transactional alloc and delete. This means that they
	// might perform some operations on transaction commit or abort
	// too.
	void *tx_malloc(size_t size);
	void tx_free(Alloced *);

	// Functions to invoke on transaction start and end.
	void mem_tx_start();
	void mem_tx_commit();
	void mem_tx_abort();
}

inline void sb7::global_init_mem() {
	GC_INIT();
}

inline void sb7::init_thread_mem() {
}

inline void *sb7::tx_malloc(size_t size) {
	return GC_MALLOC(size);
}

inline void sb7::tx_free(Alloced *ptr) {
	// do nothing and leave everything to gc
}

inline void sb7::mem_tx_start() {
	// nothing is needed
}

inline void sb7::mem_tx_commit() {
	// nothing is needed
}

inline void sb7::mem_tx_abort() {
	// nothing is needed
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
					GC_MALLOC(num * sizeof(T)));
			}

			void construct(pointer p, const T& value) {
				new((void*)p)T(value);
			}

			void destroy(pointer p) {
				p->~T();
			}

			void deallocate(pointer p, size_type num) {
				// leave it to gc
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
				return GC_MALLOC(size);
			}

			void operator delete(void* ptr) {
				// leave it to gc
			}

			virtual ~Sb7TxAlloced() { }
	};

	class Sb7Alloced : public Alloced {
		public:
			void* operator new(size_t size) {
				return GC_MALLOC(size);
			}

			void operator delete(void* ptr) {
				// leave it to gc
			}

			virtual ~Sb7Alloced() { }
	};
}

#endif
