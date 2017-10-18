/**
 * @file string.h
 * 
 * <p>
 * String class that uses sb7 memory allocator.
 * </p>
 * 
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 * @date 2007-07-13
 */

#ifndef SB7_STRING_H_
#define SB7_STRING_H_

#include <string>

#include "common/memory.h"

namespace sb7 {
	/**
	 * <p>
	 * String type to be used in the benchmark. This type is the same
	 * as standard C++ string class except that it uses Sb7Allocator
	 * for allocating memory.
	 * </p>
	 * <p>
	 * Objects of this type are NOT shareable in the sb7 environment.
	 * All objects of this type should be encapsulated in objects that
	 * are if they should be shared.
	 * </p>
	 * 
	 */
	typedef std::basic_string<char,
			std::char_traits<char>,
			sb7::Sb7Allocator<char> > string;
}

#endif /*SB7_STRING_H_*/
