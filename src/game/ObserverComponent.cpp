#include "ObserverComponent.h"

ObserverComponent::ObserverComponent()
{
	for(size_t i = 0; i < EGE_END; ++i)
	{
		m_pAction[i] = nullptr;
	}
}

bool ObserverComponent::AddEvent(const EGameEvent& event, IAction* pAction)
{
	if(m_pAction[event]){ return false; }

	m_pAction[event] = pAction;
	return true;
}

bool ObserverComponent::UpdateEvent(const EGameEvent& event, IAction* pAction)
{
	if(!m_pAction[event]){ return false; }

	m_pAction[event] = pAction;
	return true;
}

void ObserverComponent::Notify(const Event& event, EntityID currentEntity)
{
	if(m_pAction[event.event])
	{
		(*m_pAction[event.event])(currentEntity);
	}
}
