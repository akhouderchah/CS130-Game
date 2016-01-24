#pragma once

#include "../system/ISystem.h"

/**
 * @brief Takes input from SFML, "converts" it to events, and sends
 * the events out to ObserverComponents.
 */
class InputSystem : public ISystem
{
public:
	InputSystem();
	virtual ~InputSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);

private:

};

