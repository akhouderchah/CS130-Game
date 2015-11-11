#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Paths.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include <random>

enum RandomWeight
{
	ERW_VeryLikely,
	ERW_Likely,
	ERW_Average,
	ERW_Unlikely,
	ERW_Impossibru = 6
};

/**
 * @brief Small class to abstract the implementation details of pseudo-random number generation.
 */
class Random
{
public:
	static void SetSeed(unsigned long Seed);
	static int GetRand(int Max);	// Returns an int between 0 and Max
	static float GetRandRange(int start, int end);  // Returns a float between start and end
	static bool WillHappen(RandomWeight Weight);	// Returns whether or not something will happen, given a weight

private:
	static std::default_random_engine RandomEngine;
	static int RangePrecision;	// Used in RandRange for the decimal precision of floats returned

	// We don't want to create a Random class
	Random();
	~Random();
};

#endif //BASE_H
