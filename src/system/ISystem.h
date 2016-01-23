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

	static void SetWindow(sf::RenderWindow* pWindow){ m_pWindow = pWindow; }

protected:
	//static sf::Window* GetWindow(){ return m_pWindow; }

protected:
	static sf::RenderWindow* m_pWindow;
};

