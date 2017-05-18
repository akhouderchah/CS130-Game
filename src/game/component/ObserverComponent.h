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
 * that this is the case for now.
 */
class ObserverComponent : public IComponent
{
public:
	ObserverComponent(Entity entity);
	~ObserverComponent();

	void Refresh(){}

	bool Subscribe(EventSystem& pSystem);
	void Unsubscribe(EventSystem& pSystem);

	/**
	 * @brief Map an Action to a specified Event
	 *
	 * @note If the event already exists, the function returns false and
	 * doesn't change anything.
	 */
	bool AddEvent(const EGameEvent& event, IAction* pAction);

	/**
	 * @brief Map a new Action to an already-mapped Event
	 *
	 * @note If the event doesn't already have something mapped to it, this
	 * method will return false and not change anything.
	 */
	bool UpdateEvent(const EGameEvent& event, IAction* pAction);

	/**
	 * @brief Notifies the relevant Action class that the specified event occurred
	 *
	 * @note As it stands, the code does not inform the Action of which
	 * entity called it. If that were to change, IAction would have to
	 * change, as would this method.
	 */
	void Notify(const Event& event);

private:
	// Array of IAction pointers, used to map Events to Actions.
	// @note This is only acceptable if the number of game events is quite low.
	IAction* m_pAction[EGE_END];
	std::vector<EventSystem*> m_pEventSystems;
};

