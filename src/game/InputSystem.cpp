#include "InputSystem.h"
#include "InputConstants.h"

EventQueue InputSystem::s_EventQueue;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

bool InputSystem::Initialize()
{
	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::Tick(deltaTime_t dt)
{
	(void)dt;

	// Get events and such
	glfwPollEvents();

	Event event = s_EventQueue.Consume();
	while(event.event != EGE_END)
	{
		switch(event.event)
		{
			case EGE_PAUSE:
				glfwSetWindowShouldClose(s_pWindow, GL_TRUE);
				break;
			default:
				break;
		};
		event = s_EventQueue.Consume();
	}
}

void InputSystem::Inform(const Event& event)
{
	// @TODO - log error if it occurs
	s_EventQueue.PushEvent(event);
}

void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	(void)pWindow; (void)scancode; (void)mods; (void)action;
	Event event;

	// @TODO - GO THROUGH AN INPUT MAPPER!!!!
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
			case GLFW_KEY_ESCAPE:
				event.event = EGE_PAUSE;
				break;
			default:
				event.event = EGE_NONE;
		}
	}

	event.state = EGS_STARTED;

	InputSystem::Inform(event);
}

