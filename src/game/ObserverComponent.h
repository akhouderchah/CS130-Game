#pragma once

#include "IComponent.h"
#include "InputConstants.h"
#include "IAction.h"

/**
 * @brief Component to add event "listening" to an entity.
 *
 * Takes much inspiration from the Observer design pattern.
 * Maps Events to Actions (one action per event)
 * such that entities can respond to events sent out by the EventSystem.
 */
class ObserverComponent : public IComponent
{
public:
	ObserverComponent();
	~ObserverComponent(){}

	void Destroy(){}

	// Map an event to a certain Action. If the event already exists,
	// the function returns false and doesn't change anything.
	bool AddEvent(const EGameEvent& event, IAction* pAction);

	// Map a different event to a certain Action. If the event doesn't already
	// have something mapped to it, it will return false and not change anything.
	bool UpdateEvent(const EGameEvent& event, IAction* pAction);

	// The Action will need to know which entity to apply to
	void Notify(const Event& event, EntityID currentEntity);

private:
	// Array of IAction pointers, used to map Events to Actions.
	// @note This is only acceptable if the number of game events is quite low.
	IAction* m_pAction[EGE_END];
};

