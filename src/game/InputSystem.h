#pragma once

#include "../system/ISystem.h"
#include "../system/EventQueue.h"

struct Event;

void KeyCallback(GLFWwindow*, int, int, int, int);

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

	static void Inform(const Event& event);

private:
	static EventQueue s_EventQueue;
};

