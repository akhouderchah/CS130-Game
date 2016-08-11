#include "ObserverComponent.h"
#include "EventSystem.h"

ObserverComponent::ObserverComponent(EventSystem& eventSys)
{
	for(size_t i = 0; i < EGE_END; ++i)
	{
		m_pAction[i] = nullptr;
	}

	m_pEventSystem = &eventSys;
}

ObserverComponent::~ObserverComponent()
{
	Destroy();
}

void ObserverComponent::Activate()
{
	m_pEventSystem->RegisterObserver(*this);
}

void ObserverComponent::Destroy()
{
	for(size_t i = 0; i < EGE_END; ++i)
	{
		delete m_pAction[i];
		m_pAction[i] = nullptr;
	}
	
	m_pEventSystem->UnregisterObserver(*this);
}

bool ObserverComponent::AddEvent(const EGameEvent& event, IAction* pAction)
{
	if(m_pAction[event]){ delete pAction; return false; }

	m_pAction[event] = pAction;
	return true;
}

bool ObserverComponent::UpdateEvent(const EGameEvent& event, IAction* pAction)
{
	if(!m_pAction[event]){ delete pAction; return false; }

	delete m_pAction[event];
	m_pAction[event] = pAction;
	return true;
}

void ObserverComponent::Notify(const Event& event)
{
	if(m_pAction[event.event])
	{
		(*m_pAction[event.event])();
	}
}
