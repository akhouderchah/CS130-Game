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
	virtual ~ISystem(){};
	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual void Tick(deltaTime_t dt) = 0;

	/**
	 * @brief Used to allow systems interact with the window if needed.
	 *
	 * @see Game::Initialize
	 */
	static void SetWindow(GLFWwindow* pWindow){ s_pWindow = pWindow; }

protected:
	static GLFWwindow* s_pWindow;
};

