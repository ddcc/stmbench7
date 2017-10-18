/**
 * @file tid.cc
 * 
 * Definition of thread identifier generator.
 * 
 * Implemented version of thread identifier generator is very
 * simplistic, yet enough for the benchmark. If it is to be used for
 * some more general development it needs some polishing.
 * 
 * This implementation assumes that all threads will be created from
 * the same parent thread and therefore doesn't use any form of
 * synchronization for generating identifiers. Also, thread
 * initialization shouldn't be called multiple times for the same
 * thread.
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 */

#include "../thread/pthread_wrap.h"

#include "tid.h"

static unsigned next_tid = 0;

::pthread_key_t sb7::tid_key;

void sb7::init_tid_generator() {
	::pthread_key_create(&tid_key, NULL);
}

void sb7::init_thread_tid() {
	::pthread_setspecific(tid_key, (const void *)next_tid);
	next_tid++;
}
