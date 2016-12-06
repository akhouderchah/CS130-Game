#pragma once

#include "Types.h"

#include <random>

/**
 * @brief Small class to abstract the implementation details of pseudo-random number generation.
 */
class Random
{
private:
	enum RandomWeight
	{
		ERW_VeryLikely,
		ERW_Likely,
		ERW_Average,
		ERW_Unlikely,
		ERW_Impossibru = 6
	};

public:
	Random(int rangePrecision=1000) : m_RangePrecision(rangePrecision){}
	void SetSeed(unsigned long seed);
	int GetRand(int max);	// Returns an int between 0 and Max
	float GetRandRange(int start, int end);  // Returns a float between start and end
	bool WillHappen(RandomWeight weight);	// Returns whether or not something will happen, given a weight

private:
	std::default_random_engine m_RandomEngine;
	int m_RangePrecision;	// Used in RandRange for the decimal precision of floats returned
};

