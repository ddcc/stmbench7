#include <limits>

#include "common/time.h"
#include "random.h"

#ifdef SB7_TT_RANDOM_RAND_R

#include <iostream>

#define GENERATE_DOBULE_LIMIT 10000

namespace sb7 {
	Random::Random()
		: seed(get_time_ns() % 1000) { }
	
	int Random::nextInt() {
		return rand_r(&seed);
	}
	
	int Random::nextInt(int n) {
		return rand_r(&seed) % n;
	}
	
	double Random::nextDouble() {
		return nextInt(GENERATE_DOBULE_LIMIT) / (double)GENERATE_DOBULE_LIMIT;
	}
}

#elif defined SB7_TT_RANDOM_MERSENNE

namespace sb7 {
	Random::Random()
			: randomGen(TRandomMersenne(get_time_ns() % 1000)) { }

	int Random::nextInt() {
		return randomGen.IRandom(0, std::numeric_limits<int>::max());
	}

	int Random::nextInt(int n) {
		return randomGen.IRandom(0, n - 1);
	}

	double Random::nextDouble() {
		return randomGen.Random();
	}
}

#endif /* rand implementation */

pthread_key_t sb7::random_key;

void sb7::global_init_random() {
	::pthread_key_create(&random_key, NULL);
}

void sb7::thread_init_random() {
	::pthread_setspecific(random_key,
		(const void *)(new sb7::Random()));
}
