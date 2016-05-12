#include "Base.h"

/**
 * Random class static definitions
 */
std::default_random_engine Random::RandomEngine;
int Random::RangePrecision = 1000;
/**/

/**
 * GUID class family static definitions
 */
template <class T, class IDType>
IDType GUID<T, IDType>::s_currentID = 1;
/**/

void Random::SetSeed(unsigned long Seed)
{
	RandomEngine.seed(Seed);
}

int Random::GetRand(int Max)
{
	return RandomEngine() % ( Max + 1 );
}

float Random::GetRandRange(int start, int end)
{
	return ((RandomEngine() % (RangePrecision * (end-start))) / float(RangePrecision)) + start;
}

bool Random::WillHappen(RandomWeight Weight)
{
	// Is W^2+1 < .95?
	// @TODO - Use real probability distribution!
	return (GetRandRange(0, (Weight * Weight) + 1) < 0.95f);
}
