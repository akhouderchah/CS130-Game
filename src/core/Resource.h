#pragma once

#include "../base/Base.h"

/**
 * @brief Defines the basic interface and functionality for all resources (assets loaded from files).
 *
 * Could be used so that a ResourceManager takes care of loading
 * and reusing resources.
 */
class Resource
{
public:
	virtual ~Resource(){}

};

