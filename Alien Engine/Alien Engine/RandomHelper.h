#ifndef RANDOM
#define RANDOM

#include "PCG/pcg_random.hpp"
#include <random>

class __declspec(dllexport) Random {
public:

	// min & max are inclusive
	static int GetRandomIntBetweenTwo(const int& min, const int& max) {
		pcg_extras::seed_seq_from<std::random_device> seed_source;
		pcg32 rng(seed_source);
		std::uniform_int_distribution<int> uniform_dist(min, max);
		return uniform_dist(rng);
	}

	static u64 GetRandomID() {
		pcg_extras::seed_seq_from<std::random_device> seed_source;
		pcg32 rng(seed_source);
		std::uniform_int_distribution<u64> uniform_dist(1, ULLONG_MAX);
		return uniform_dist(rng);
	}

	// min & max are inclusive
	static float GetRandomFloatBetweenTwo(const float& min, const float& max) {
		pcg_extras::seed_seq_from<std::random_device> seed_source;
		pcg32 rng(seed_source);
		std::uniform_real_distribution<float> uniform_dist(min, max);
		return uniform_dist(rng);
	}
};




#endif // !RANDOM




