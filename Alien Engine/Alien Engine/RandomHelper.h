#ifndef RANDOM
#define RANDOM

#include "PCG/pcg_random.hpp"
#include <random>

int GetRandomIntBetweenTwo(const int& min, const int& max) {
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg32 rng(seed_source);
	std::uniform_int_distribution<int> uniform_dist(min, max);
	return uniform_dist(rng);
}

float GetRandomFloatBetweenTwo(const float& min, const float& max) {

	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg32 rng(seed_source);
	std::uniform_real_distribution<float> uniform_dist(min, max);
	return uniform_dist(rng);
}


#endif // !RANDOM




