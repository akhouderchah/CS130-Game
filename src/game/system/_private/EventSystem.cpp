#include "EventSystem.h"
#include "InputConstants.h"
#include "ObserverComponent.h"

EventSystem* EventSystem::s_pInputSystem = nullptr;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

bool EventSystem::Initialize()
{
	return true;
}

void EventSystem::Shutdown()
{
	UnmakeInputSystem();
}

void EventSystem::Tick(deltaTime_t dt)
{
	(void)dt;

	// Get events and such
	glfwPollEvents();

	Event event = m_EventQueue.Consume();
	while(event.event != EGE_END)
	{
		for(size_t i = 0; i < m_pObservers.size(); ++i)
		{
			m_pObservers[i]->Notify(event);
		}
		event = m_EventQueue.Consume();
	}
}

bool EventSystem::MakeInputSystem()
{
	if(s_pInputSystem){ return false; }

	s_pInputSystem = this;
	return true;
}

void EventSystem::UnmakeInputSystem()
{
	if(s_pInputSystem == this)
	{
		s_pInputSystem = nullptr;
	}
}

bool EventSystem::RegisterObserver(ObserverComponent& observer)
{
	// In debug mode, check to see if we're adding an observer multiple times
	#ifdef _DEBUG
	for(size_t i = 0; i < m_pObservers.size(); ++i)
	{
		if(m_pObservers[i] == &observer)
		{
			LOG_SPECIAL("WARNING", "The observer at memory location: " << &observer << " is being registered multiple times to the same EventSystem!\n");
		}
	}
	#endif

	m_pObservers.push_back(&observer);

	return true;
}

void EventSystem::UnregisterObserver(ObserverComponent& observer)
{
	for(size_t i = 0; i < m_pObservers.size(); ++i)
	{
		if(m_pObservers[i] == &observer)
		{
			std::swap(m_pObservers[i], m_pObservers.back());
			m_pObservers.pop_back();
			return;
		}
	}
}

void EventSystem::Inform(const Event& event)
{
	// @TODO - log error if it occurs
	m_EventQueue.PushEvent(event);
}

void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	(void)pWindow; (void)scancode; (void)mods; (void)action;
	Event event;

	assert(EventSystem::s_pInputSystem);

	// @TODO - GO THROUGH AN INPUT MAPPER!!!!
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
		case GLFW_KEY_ESCAPE:
			event.event = EGE_PAUSE;
			break;
		case GLFW_KEY_Z:
			event.event = EGE_PLAYER1_JUMP;
			break;
		case GLFW_KEY_X:
			event.event = EGE_PLAYER2_JUMP;
			break;
		case GLFW_KEY_C:
			event.event = EGE_PLAYER3_JUMP;
			break;
		case GLFW_KEY_V:
			event.event = EGE_PLAYER4_JUMP;
			break;
		default:
			event.event = EGE_NONE;
		}

		event.state = EGS_STARTED;
		EventSystem::s_pInputSystem->Inform(event);
	}
}

