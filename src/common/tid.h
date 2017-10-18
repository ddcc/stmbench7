/**
 * @file tid.h
 * 
 * Declaration of elements required for thread identifier management
 * are in this file.
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 * @date 2007-07-11
 */

#ifndef SB7_TID_H_
#define SB7_TID_H_

#include <pthread.h>
#include <stdint.h>

namespace sb7 {
	void init_tid_generator();

	void init_thread_tid();

	unsigned get_tid();

	extern ::pthread_key_t tid_key;
}

inline unsigned sb7::get_tid() {
	uint64_t tmp = (uint64_t)::pthread_getspecific(tid_key);
	return (unsigned)tmp;
}

#endif
