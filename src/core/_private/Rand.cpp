#include "Rand.h"

void Random::SetSeed(unsigned long seed)
{
	m_RandomEngine.seed(seed);
}

int Random::GetRand(int max)
{
	return m_RandomEngine() % (max + 1);
}

float Random::GetRandRange(int start, int end)
{
	return ((m_RandomEngine() % (m_RangePrecision * (end-start))) / float(m_RangePrecision)) + start;
}

bool Random::WillHappen(RandomWeight weight)
{
	// Is W^2+1 < .95?
	// @TODO - Use real probability distribution!
	return (GetRandRange(0, (weight * weight) + 1) < 0.95f);
}

