#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Paths.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include <random>
#include <cstdint>
#include <cassert>

/**
 * Integer typedefs
 */
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

/**
 * Enumeration to be used with the Random class
 *
 * @TODO - Make an enum class?
 */
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

/**
 * @brief Set of classes to generate GUIDs for different purposes.
 *
 * @in - T - The GUID "purpose" (the set of classes (base class perhaps) that will use the GUID)
 * @in - IDType - The data type of the ID (most likely an integer type, although it could be specialized
 *	different types)
 *
 *	@note The base implementation only generates IDs, it does not reclaim them, even if an ID is no longer used.
 */
template <class T, class IDType>
class GUID
{
public:
	static IDType GenerateID()
	{
		assert(s_currentID+1 > s_currentID);	// Check for overflow
		return s_currentID++;
	}

private:
	static IDType s_currentID;
	GUID();
	~GUID();
};

