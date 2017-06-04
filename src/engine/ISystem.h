#pragma once

#include "IComponent.h"
#include "ComponentManager.h"
#include "InputConstants.h"
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
	bool SetGameState(EGameState State) {
		if (State!=*s_pState)
		{*s_pState=EGS_PAUSED;
		return true;}
		*s_pState=EGS_STARTED;
		return false;
	};
	EGameState GetGameState() {return *s_pState;};
	/**
	 * @brief Used to allow systems interact with the window if needed.
	 *
	 * @see Game::Initialize
	 */
	static void SetWindow(GLFWwindow* pWindow){ s_pWindow = pWindow; };
	static void InitState(EGameState* pState){s_pState = pState; }
protected:
	static GLFWwindow* s_pWindow;
	static EGameState* s_pState;
};

