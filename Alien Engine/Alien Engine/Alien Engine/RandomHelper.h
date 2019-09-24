#pragma once

#include "PCG/pcg_random.hpp"
#include <random>

int GetRandomIntBetweenTwo(const int& min, const int& max) {
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg32 rng(seed_source);
	std::uniform_int_distribution<int> uniform_dist(min, max);
	return uniform_dist(rng);
}

// floats must have one decimal
float GetRandomFloatBetweenTwo(const float& min, const float& max) { 

	float min_ = min, max_ = max;
	int count = 0;
	while (min_ != (int)min_) {
		min_ *= 10;
		max_ *= 10;
		count++;
	}

	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg32 rng(seed_source);
	std::uniform_int_distribution<int> uniform_dist((int)min_, (int)max_);
	return ((float)(uniform_dist(rng) / (float)(count * 10)));
}

