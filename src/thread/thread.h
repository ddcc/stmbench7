#ifndef SB7_THREAD_H_
#define SB7_THREAD_H_

/**
 * @file thread.h
 * 
 * This file contains definitions of things that are related to thread
 * management and are not defined in appropriate way in tl2 itself.
 * Mainly one single function to initialize all needed data structures
 * and one for thread init and cleanup.
 * 
 */

#include "../random.h"

namespace sb7 {

	/**
	 * Initialize data shared by all threads.
	 */
	void global_thread_init();

	/**
	 * Initialize specific thread.
	 */
	void thread_init();

	/**
	 * Special initialization for init thread.
	 */
	void init_thread_init();

	/**
	 * Clean after the thread.
	 */
	void thread_clean();
}

#endif /*SB7_THREAD_H_*/
