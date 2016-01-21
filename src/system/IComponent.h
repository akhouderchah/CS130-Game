#pragma once

#include "../base/Base.h"

/*
 * @brief Base class for all components
 *
 * @note Users should NOT store component pointers or references,
 * as its respective ComponentManager may rearrange components in
 * memory during program execution.
 */
class IComponent
{
public:
	virtual ~IComponent(){}
	virtual void Tick(deltaTime_t dt) = 0;
};

