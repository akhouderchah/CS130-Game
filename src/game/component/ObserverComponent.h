#pragma once

#include "IComponent.h"
#include "InputConstants.h"
#include "IAction.h"

class EventSystem;

/**
 * @brief Component to add event listening to an entity.
 *
 * Takes inspiration from the Observer design pattern.
 * Maps Events to Actions (one action per event)
 * such that entities can respond to events sent out by the EventSystem.
 *
 * @note The current implementation only allows an ObserverComponent one
 * EventSystem to observe. This could easily be changed later, but take note
 * that this is the case.
 *
 * @todo Activate() is an ugly workaround of the fact that the EventSystem
 * stores ObserverComponent*s, while the ComponentManager passes around component
 * rvalues. Thus if we give the EventSystem "this" on construction, then the pointer
 * will no longer be valid after the ComponentManager finishes moving the component.
 */
class ObserverComponent : public IComponent
{
public:
	ObserverComponent(Entity entity);
	~ObserverComponent();

	void Refresh(){}

	bool Subscribe(EventSystem& pSystem);
	void Unsubscribe(EventSystem& pSystem);

	// Map an event to a certain Action. If the event already exists,
	// the function returns false and doesn't change anything.
	bool AddEvent(const EGameEvent& event, IAction* pAction);

	// Map a different event to a certain Action. If the event doesn't already
	// have something mapped to it, it will return false and not change anything.
	bool UpdateEvent(const EGameEvent& event, IAction* pAction);

	// The Action will need to know which entity to apply to
	void Notify(const Event& event);

private:
	// Array of IAction pointers, used to map Events to Actions.
	// @note This is only acceptable if the number of game events is quite low.
	IAction* m_pAction[EGE_PLAYER2_JUMP];
	std::vector<EventSystem*> m_pEventSystems;
};

