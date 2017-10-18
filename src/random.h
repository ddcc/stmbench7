#ifndef SB7_RANDOM_H_
#define SB7_RANDOM_H_

#define SB7_TT_RANDOM_RAND_R
//#define SB7_TT_RANDOM_MERSENNE

#ifdef SB7_TT_RANDOM_RAND_R

#include <cstdlib>

#include "common/memory.h"
#include "randomc/randomc.h"

namespace sb7 {
	class Random : public Sb7Alloced {
	public:
		Random();
		
		int nextInt();
		
		int nextInt(int n);
		
		double nextDouble();
		
	private:
		unsigned seed;
	};
}

#elif defined SB7_TT_RANDOM_MERSENNE

#include "thread/pthread_wrap.h"
#include "common/memory.h"
#include "randomc/randomc.h"

namespace sb7 {
	class Random : public Sb7Alloced {
		public:
			Random();

			int nextInt();

			int nextInt(int n);

			double nextDouble();

		private:
			TRandomMersenne randomGen;
	};
}

#endif /* SB7_TT_RANDOM_MERSENNE */

namespace sb7 {
	
	extern ::pthread_key_t random_key;

	// get thread local random object
	Random *get_random();

	void global_init_random();
	void thread_init_random();
}

inline sb7::Random *sb7::get_random() {
	return (sb7::Random *)::pthread_getspecific(random_key);
}

#endif /*SB7_RANDOM_H_*/
