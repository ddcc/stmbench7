/**
 * @file tm_ptr.h
 * 
 * Declaration of smart pointer general for different word based stms.
 * The class names and relations were influenced by rstm because
 * original benchmark was developed for rstm api.
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 * @date 2007-07-12
 */

#ifndef SB7_TM_PTR_H_
#define SB7_TM_PTR_H_

#include <map>

#include "tm_spec.h"
#include "../common/memory.h"

namespace sb7 {

	// wrapper for a pointer that can be transactionally deleted
	template<typename T>
	class Handle : public Sb7TxAlloced {
		public:
			T *obj;
	};
	

	// All interactions with memory locations that are accessed by
	// different threads are done using tm specific interface and they
	// are never read directly. This is why there is no need for any
	// of the pointers to be declared as volatile.

	// base object that all application objects will inherit from
	template <typename T>
	class Object : public sb7::Sb7TxAlloced {
		public:
			Object() : handle(NULL) { }

			// make sure that handle gets copied with the object
			// as well
			T* cloneFull() {
				T *ret = clone();
				((Object<T> *)ret)->handle = handle;
				return ret;
			}

			virtual T* clone() const = 0;

			virtual ~Object() { }

			Handle<T> *getHandle() {
				// this will happen only when the object is still
				// "private" to one transaction, so it is safe to do
				// it like this
				if(handle == NULL) {
					handle = new Handle<T>();
					handle->obj = (T *)this;
				}

				return handle;
			}

		protected:
			// I tried to avoid this, but it would be too complex to do
			// so.
			//
			// It is quite important :) to avoid having different
			// object handles for the same object. It could be avoided
			// by coding everything very carefully, but this would beat
			// the point of creating the rstm-like API on top of tl2
			// in the first place.
			Handle<T> *handle;

			// TODO think about avoiding manual search through the log
			// by adding another flag field here and using it to
			// distinguish already logged version from the one that
			// still needs to be logged.
			//
			// If this is done, objects in the log should have this
			// flag reset before the commit.
			//
			// Probably don't do this.
	};
}

// I am going to need log of all objects that were written. I don't see
// a way to use tl2 interface to figure out if the returned object copy
// was already written, or if it needs writing.
namespace sb7 {
	class ObjectLog : public Sb7Alloced {
		// as this file already uses memory management, it is fine to
		// use STL map implementation with our allocator
		typedef std::map<void *, void *, std::less<void *>,
			Sb7Allocator<std::pair<void * const, void *> > > obj_log;

		public:
			ObjectLog() : parent_log(NULL) {
				// empty
			}

			void put(void *addr, void *clone);

			void *get(void *addr);

			void clear();

			void merge(ObjectLog *pl);

			ObjectLog *getParentLog() {
				return parent_log;
			}

			void setParentLog(ObjectLog *pl) {
				parent_log = pl;
			}

		private:
			obj_log log;

			ObjectLog *parent_log;
	};

	// this again requires thread local obj log
	extern ::pthread_key_t obj_log_key;

	ObjectLog *getObjectLog();
	void obj_log_tx_start(ObjectLog *pl = NULL);
	void obj_log_tx_commit();
	void obj_log_tx_abort();

	void global_init_obj_log();
	void thread_init_obj_log();
}

inline void sb7::ObjectLog::put(void *addr, void *clone) {
	log[addr] = clone;
}

inline void *sb7::ObjectLog::get(void *addr) {
	obj_log::iterator iter = log.find(addr);
	return (iter == log.end()) ? NULL : (*iter).second;
}

inline void sb7::ObjectLog::clear() {
	log.clear();
}

inline void sb7::ObjectLog::merge(ObjectLog *log_to_merge) {
	obj_log *l = &log_to_merge->log;

	for(obj_log::iterator i = l->begin();i != l->end();i++) {
		log.erase((*i).first);
		put((*i).first, (*i).second);
	}
}


inline sb7::ObjectLog *sb7::getObjectLog() {
	return (ObjectLog *)::pthread_getspecific(obj_log_key);
}

inline void sb7::obj_log_tx_start(ObjectLog *pl) {
	ObjectLog *log = getObjectLog();
	log->setParentLog(pl);
}

inline void sb7::obj_log_tx_commit() {
	ObjectLog *log = getObjectLog();

	if(log->getParentLog() != NULL) {
		log->getParentLog()->merge(log);
	}

	getObjectLog()->clear();
}

inline void sb7::obj_log_tx_abort() {
	getObjectLog()->clear();
}

// declaration of shared pointer type
// some parts of this code are taken from rstm source code
namespace sb7 {
	template <typename> class sh_ptr;
	template <typename> class rd_ptr;
	template <typename> class wr_ptr;
	template <typename> class notx_ptr;

	template <typename T> void tx_delete(
		sh_ptr<T>& delete_on_tx_commit);

	template <typename T>
	class sh_ptr {
		public:
			friend class rd_ptr<T>;
			friend class wr_ptr<T>;
			friend class notx_ptr<T>;

			friend void tx_delete<>(sh_ptr<T>&);

		public:
			sh_ptr();
			explicit sh_ptr(T *);
			explicit sh_ptr(Handle<T> *);
			sh_ptr(const sh_ptr<T>& copy);

			sh_ptr<T>& operator=(const int must_be_null);
			sh_ptr<T>& operator=(const sh_ptr<T>& assign);

			bool operator==(const void*) const;
			bool operator!=(const void*) const;
			bool operator==(const sh_ptr<T>&) const;
			bool operator!=(const sh_ptr<T>&) const;

			// so that sh_ptr<T>s can be stored in STL collections
			bool operator<(const sh_ptr<T>& rhs) const;

			template <typename T2> operator sh_ptr<T2>();

		private:
			// handle of an inner object that has to be inherited from
			// Object<T>
			Handle<T> *obj;
	};

    template <typename T>
    class rd_ptr {
		friend class sh_ptr<T>;
		friend class wr_ptr<T>;

		public:
			// To facilitate private/txnal method templates:
			typedef wr_ptr<T> upgrade;
			template<typename O> struct r_analogue {
				typedef rd_ptr<O> type;
			};
			template<typename O> struct w_analogue {
				typedef wr_ptr<O> type;
			};

			rd_ptr(const int must_be_null);
			rd_ptr(const rd_ptr<T>& copy);
			explicit rd_ptr(const sh_ptr<T>& to_open);

			rd_ptr<T>& operator=(const int must_be_null);
			rd_ptr<T>& operator=(const rd_ptr<T>& assign);
			rd_ptr<T>& operator=(const sh_ptr<T>& to_open);

			const T* operator->() const;
			const T& operator*() const;

			bool operator==(const void*) const;
			bool operator!=(const void*) const;
			bool operator==(const rd_ptr<T>&) const;
			bool operator!=(const rd_ptr<T>&) const;
			bool operator==(const sh_ptr<T>&) const;
			bool operator!=(const sh_ptr<T>&) const;

		private:
			const T* open_shared(const sh_ptr<T> &shared);

		private:
			mutable const T* obj;
    };

    template <typename T>
    class wr_ptr {
		friend class sh_ptr<T>;
		friend class rd_ptr<T>;

		public:
			// To facilitate private/txnal method templates:
			template<typename O> struct r_analogue {
				typedef rd_ptr<O> type;
			};
			template<typename O> struct w_analogue {
            	typedef wr_ptr<O> type;
			};

			wr_ptr(const int must_be_null);
			wr_ptr(const wr_ptr<T>&);
			explicit wr_ptr(const sh_ptr<T>&);

			wr_ptr<T>& operator=(const int must_be_null);
			wr_ptr<T>& operator=(const wr_ptr<T>&);
			wr_ptr<T>& operator=(const sh_ptr<T>&);

			T* operator->() const;
			T& operator*() const;

			bool operator==(const void*) const;
			bool operator!=(const void*) const;
			bool operator==(const wr_ptr<T>&) const;
			bool operator!=(const wr_ptr<T>&) const;

		private:
			T* open_shared(const sh_ptr<T> &shared);

		private:
			mutable T* obj;
    };

    template <typename T>
    class notx_ptr {
		friend class sh_ptr<T>;
		
	public:
		// To facilitate private/txnal method templates:
		template<typename O> struct r_analogue {
			typedef notx_ptr<O> type;
		};
		
		notx_ptr(const int must_be_null);
		notx_ptr(const notx_ptr<T>& copy);
		explicit notx_ptr(const sh_ptr<T>& to_open);
		
		notx_ptr<T>& operator=(const int must_be_null);
		notx_ptr<T>& operator=(const notx_ptr<T>& assign);
		notx_ptr<T>& operator=(const sh_ptr<T>& to_open);
		
		const T* operator->() const;
		const T& operator*() const;
		
		bool operator==(const void*) const;
		bool operator!=(const void*) const;
		bool operator==(const notx_ptr<T>&) const;
		bool operator!=(const notx_ptr<T>&) const;
		bool operator==(const sh_ptr<T>&) const;
		bool operator!=(const sh_ptr<T>&) const;
		
	private:
		const T* open_shared(const sh_ptr<T> &shared);
		
	private:
		mutable const T* obj;
    };	
}

namespace sb7 {

	template <typename T>
	inline sh_ptr<T>::sh_ptr() : obj(NULL) { }

	template <typename T>
	inline sh_ptr<T>::sh_ptr(Handle<T> *o) : obj(o) { }

	template <typename T>
	inline sh_ptr<T>::sh_ptr(T* o) {
		obj = (Handle<T> *)o->getHandle();
	}

	template <typename T>
	inline sh_ptr<T>::sh_ptr(const sh_ptr<T>& ptr) : obj(ptr.obj) {	}

    template <typename T>
    template <typename T2>
    inline sh_ptr<T>::operator sh_ptr<T2>() {
        return sh_ptr<T2>((Handle<T2> *)obj);
    }

	template <typename T>
	inline sh_ptr<T>& sh_ptr<T>::operator=(const int must_be_null) {
		assert(must_be_null == 0);
		obj = NULL;
		return *this;
	}

	template <typename T>
	inline sh_ptr<T>& sh_ptr<T>::operator=(const sh_ptr<T>& rhs) {
		obj = rhs.obj;
		return *this;
	}

	template <typename T>
	inline bool sh_ptr<T>::operator==(const void* rhs) const {
		return (obj == rhs);
	}

	template <typename T>
	inline bool sh_ptr<T>::operator!=(const void* rhs) const {
		return (obj != rhs);
	}

	template <typename T>
	inline bool sh_ptr<T>::operator==(const sh_ptr<T>& rhs) const {
		return (obj == rhs.obj);
	}

	template <typename T>
	inline bool sh_ptr<T>::operator!=(const sh_ptr<T>& rhs) const {
		return (obj != rhs.obj);
	}

	template <typename T>
	inline bool sh_ptr<T>::operator<(const sh_ptr<T>& rhs) const {
		return (obj < rhs.obj);
	}

	template <typename T>
	void tx_delete(sh_ptr<T> &ptr) {
		void *addr = (void *)&(ptr.obj->obj);

		// delete object itself
		// but make sure that you are deleting the correct version of
		// the object
		tx_free((Alloced *)tm_read_word(addr));

		// then delete handle of the object
		tx_free(ptr.obj);

		// just to cause conflict with other transactions
		tm_write_word(addr, NULL);
	}
}

namespace sb7 {

	template <typename T>
	inline const T *rd_ptr<T>::open_shared(const sh_ptr<T>& shared) {
		return (const T *)tm_read_word((void *)&(shared.obj->obj));
	}

	template <typename T>
	inline rd_ptr<T>::rd_ptr(const int must_be_null) : obj(NULL) {
		assert(must_be_null == 0);
	}

	template <typename T>
	inline rd_ptr<T>::rd_ptr(const rd_ptr<T>& ptr) : obj(ptr.obj) { }

	template <typename T>
	inline rd_ptr<T>::rd_ptr(const sh_ptr<T>& shared) {
		obj = open_shared(shared);
	}

	template <typename T>
	inline rd_ptr<T>& rd_ptr<T>::operator=(const int must_be_null) {
		assert(must_be_null == 0);
		obj = NULL;
		return *this;
	}

	template <typename T>
	inline rd_ptr<T>& rd_ptr<T>::operator=(const rd_ptr<T>& rhs) {
		obj = rhs.obj;
		return *this;
	}

	template <typename T>
	inline rd_ptr<T>& rd_ptr<T>::operator=(const sh_ptr<T>& rhs) {
		obj = open_shared(rhs);
		return *this;
	}

	template <typename T>
	const T* rd_ptr<T>::operator->() const {
		return obj;
	}

	template <typename T>
	const T& rd_ptr<T>::operator*() const {
		return *obj;
	}

	template <typename T>
	inline bool rd_ptr<T>::operator==(const void* rhs) const {
		return (obj == rhs);
	}

	template <typename T>
	inline bool rd_ptr<T>::operator!=(const void* rhs) const {
		return (obj != rhs);
	}

	template <typename T>
	inline bool rd_ptr<T>::operator==(const rd_ptr<T>& rhs) const {
		return (obj == rhs.obj);
	}

	template <typename T>
	inline bool rd_ptr<T>::operator!=(const rd_ptr<T>& rhs) const {
		return (obj != rhs.obj);
	}
}

namespace sb7 {

	template <typename T>
	inline T *wr_ptr<T>::open_shared(const sh_ptr<T>& ptr) {
		ObjectLog *obj_log = getObjectLog();
		void *addr = (void *)&(ptr.obj->obj); // addr is address of handle
		void *ret;

		// first look if the object was already opened for writing
		ret = obj_log->get(addr);

		// if object was not written so far make a clone of it,
		// write new pointer value and mark it as written
		// old version of the object should be deleted if the
		// transaction commits successfully
		if(ret == NULL) {
			// val is current address of object
			void *val = tm_read_word(addr);

			// clone current object and save it as a new value
			ret = ((Object<T> *)val)->cloneFull();
			tm_write_word(addr, ret);
			obj_log->put(addr, ret);

			// delete old object's version if the transaction finishes
			// successfully
			tx_free((Alloced *)val);
		}

		return (T *)ret;
	}

	template <typename T>
	inline wr_ptr<T>::wr_ptr(const int must_be_null) : obj(NULL) {
		assert(must_be_null == 0);
	}

	template <typename T>
	inline wr_ptr<T>::wr_ptr(const wr_ptr<T>& ptr) : obj(ptr.obj) { }

	template <typename T>
	inline wr_ptr<T>::wr_ptr(const sh_ptr<T>& ptr) {
		obj = open_shared(ptr);
	}

	template <typename T>
	inline wr_ptr<T>& wr_ptr<T>::operator=(const int must_be_null) {
		assert(must_be_null == 0);
        obj = NULL;
		return *this;
    }

	template <typename T>
	inline wr_ptr<T>& wr_ptr<T>::operator=(const wr_ptr<T>& ptr) {
		obj = ptr.obj;
		return *this;
	}

	template <typename T>
	inline wr_ptr<T>& wr_ptr<T>::operator=(const sh_ptr<T>& ptr) {
		obj = open_shared(ptr);
		return *this;
	}

	template <typename T>
	T* wr_ptr<T>::operator->() const {
		return obj;
	}

	template <typename T>
	T& wr_ptr<T>::operator*() const {
		return *obj;
	}

	template <typename T>
	inline bool wr_ptr<T>::operator==(const void* rhs) const {
		return (obj == rhs);
	}

	template <typename T>
	inline bool wr_ptr<T>::operator!=(const void* rhs) const {
		return (obj != rhs);
	}

	template <typename T>
	inline bool wr_ptr<T>::operator==(const wr_ptr<T>& rhs) const {
		return (obj == rhs.obj);
	}

	template <typename T>
	inline bool wr_ptr<T>::operator!=(const wr_ptr<T>& rhs) const {
		return (obj != rhs.obj);
	}
}

namespace sb7 {
	
	template <typename T>
	inline const T *notx_ptr<T>::open_shared(const sh_ptr<T>& shared) {
		return (const T *)(shared.obj->obj);
	}
	
	template <typename T>
	inline notx_ptr<T>::notx_ptr(const int must_be_null) : obj(NULL) {
		assert(must_be_null == 0);
	}
	
	template <typename T>
	inline notx_ptr<T>::notx_ptr(const notx_ptr<T>& ptr) : obj(ptr.obj) { }
	
	template <typename T>
	inline notx_ptr<T>::notx_ptr(const sh_ptr<T>& shared) {
		obj = open_shared(shared);
	}	

	template <typename T>
	inline notx_ptr<T>& notx_ptr<T>::operator=(const int must_be_null) {
		assert(must_be_null == 0);
		obj = NULL;
		return *this;
	}
	
	template <typename T>
	inline notx_ptr<T>& notx_ptr<T>::operator=(const notx_ptr<T>& rhs) {
		obj = rhs.obj;
		return *this;
	}
	
	template <typename T>
	inline notx_ptr<T>& notx_ptr<T>::operator=(const sh_ptr<T>& rhs) {
		obj = open_shared(rhs);
		return *this;
	}
	
	template <typename T>
	const T* notx_ptr<T>::operator->() const {
		return obj;
	}
	
	template <typename T>
	const T& notx_ptr<T>::operator*() const {
		return *obj;
	}
	
	template <typename T>
	inline bool notx_ptr<T>::operator==(const void* rhs) const {
		return (obj == rhs);
	}
	
	template <typename T>
	inline bool notx_ptr<T>::operator!=(const void* rhs) const {
		return (obj != rhs);
	}
	
	template <typename T>
	inline bool notx_ptr<T>::operator==(const notx_ptr<T>& rhs) const {
		return (obj == rhs.obj);
	}
	
	template <typename T>
	inline bool notx_ptr<T>::operator!=(const notx_ptr<T>& rhs) const {
		return (obj != rhs.obj);
	}
}


#endif
