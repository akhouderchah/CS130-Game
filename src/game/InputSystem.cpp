#include "InputSystem.h"

using namespace sf;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

bool InputSystem::Initialize()
{
}

void InputSystem::Shutdown()
{
}

void InputSystem::Tick(deltaTime_t dt)
{
	(void)dt;

	Event event;
	while(m_pWindow && m_pWindow->pollEvent(event))
	{
		switch(event.type)
		{
			case Event::Closed:
				m_pWindow->close();
				break;
			case Event::KeyPressed:
				if(event.key.code == Keyboard::Escape)
				{
					m_pWindow->close();
				}
				break;
			default:
				break;
		}
	}
}
