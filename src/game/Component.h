#pragma once

/*
 * @brief Base class for all components
 *
 * @note Users should NOT store component pointers or references,
 * as its respective ComponentManager may rearrange components in
 * memory during program execution.
 */
class Component
{
public:
	virtual void Tick() = 0;
};

