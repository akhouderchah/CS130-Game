#pragma once

#include "IComponent.h"
#include "InputConstants.h"
#include "IAction.h"

/**
 * @brief Component to add event "listening" to an entity.
 *
 * Takes much inspiration from the Observer design pattern.
 * Maps Events to Actions (one action per event)
 * such that entities can respond to events sent out by the InputSystem.
 */
class ObserverComponent : public IComponent
{
public:
	ObserverComponent(){}
	~ObserverComponent(){}

	void Destroy(){}

	// Map an event to a certain Action. If the event already exists,
	// the function returns false and doesn't change anything.
	bool AddEvent(const EGameEvent& event, IAction* pAction);

	bool UpdateEvent(const EGameEvent& event, IAction* pAction);

	// The Action will need to know which entity to apply to
	bool Notify(const Event& event, EntityID currentEntity);
};

