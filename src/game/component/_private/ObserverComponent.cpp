#include "ObserverComponent.h"
#include "EventSystem.h"

ObserverComponent::ObserverComponent(Entity entity) :
	IComponent(entity)
{
	for(size_t i = 0; i < EGE_PLAYER2_JUMP; ++i)
	{
		m_pAction[i] = nullptr;
	}
}

ObserverComponent::~ObserverComponent()
{
	for(size_t i = 0; i < EGE_PLAYER2_JUMP; ++i)
	{
		delete m_pAction[i];
	}

	for(size_t i = 0; i < m_pEventSystems.size(); ++i)
	{
		Unsubscribe(*m_pEventSystems[i]);
	}
}

bool ObserverComponent::Subscribe(EventSystem& pSystem)
{
	return pSystem.RegisterObserver(*this);
}

void ObserverComponent::Unsubscribe(EventSystem& pSystem)
{
	pSystem.UnregisterObserver(*this);
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
