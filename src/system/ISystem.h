#pragma once

#include "IComponent.h"
#include "ComponentManager.h"
#include <cstddef>

/**
 * @brief Interface for all systems
 */
class ISystem
{
public:
	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual void Tick(deltaTime_t dt) = 0;
};

