/**
 * @file time.h
 * 
 * All functions related to time are defined in this header file. The
 * principle behind is to use as light-weight wrappers as possible.
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 * @date 2007-07-13
 */

#ifndef SB7_TIME_H_
#define SB7_TIME_H_

#include <stdint.h>

namespace sb7 {
	
	/**
	 * Get time in ms since epoch. This function might be architecture
	 * specific.
	 */
	uint64_t get_time_ms();

	/**
	 * Get time in us since epoch. This function might be architecture
	 * specific.
	 */
	uint64_t get_time_ns();

	/**
	 * Convenience function for delaying thread for time interval
	 * that is specified in milliseconds.
	 */
	bool sleep(uint64_t msec);
}

#define MILLISECONDS_IN_SECOND 1000
#define NANOSECONDS_IN_MILLISECOND 1000000

#if defined SB7_TT_LINUX || defined SB7_TT_SOLARIS

#define NANOSECONDS_IN_SECOND 1000000000

#include <ctime>
#include <sys/time.h>

// define functions as inline here
inline uint64_t sb7::get_time_ms() {
	struct ::timespec t;
	::clock_gettime(CLOCK_REALTIME, &t);
	return t.tv_sec * MILLISECONDS_IN_SECOND +
		t.tv_nsec / NANOSECONDS_IN_MILLISECOND;
}

inline uint64_t sb7::get_time_ns() {
	struct ::timespec t;
	::clock_gettime(CLOCK_REALTIME, &t);
	return t.tv_sec * NANOSECONDS_IN_SECOND + t.tv_nsec;
}

inline bool sb7::sleep(uint64_t msec) {
	struct ::timespec req;
	req.tv_sec = msec / MILLISECONDS_IN_SECOND;
	req.tv_nsec = (msec % MILLISECONDS_IN_SECOND)
		* NANOSECONDS_IN_MILLISECOND;

	return ::clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL) != -1;
}

#elif defined SB7_TT_MACOS

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <time.h>

inline uint64_t sb7::get_time_ms() {
	return get_time_ns() / NANOSECONDS_IN_MILLISECOND;
}

inline uint64_t sb7::get_time_ns() {
	uint64_t time = mach_absolute_time();
	Nanoseconds nano = AbsoluteToNanoseconds(*(AbsoluteTime *)&time);
	return *(uint64_t *)&nano;
}

inline bool sb7::sleep(uint64_t msec) {
	struct ::timespec req;
	req.tv_sec = msec / MILLISECONDS_IN_SECOND;
	req.tv_nsec = (msec % MILLISECONDS_IN_SECOND)
		* NANOSECONDS_IN_MILLISECOND;
	
	return ::nanosleep(&req, NULL) != -1;	
}

#endif /* LINUX */

#endif
