#pragma once

#include "Action_PauseGame.h"
#include "Action_Jump.h"
#include "EventQueue.h"
#include "ISystem.h"

struct Event;
class ObserverComponent;

void KeyCallback(GLFWwindow*, int, int, int, int);

/**
 * @brief Takes input from SFML, "converts" it to events, and sends
 * the events out to ObserverComponents.
 *
 * Multiple systems can be created to handle different types of events (since
 * registered Observers will get every event from an EventSystem, regardless of
 * their relevance).
 *
 * ObserverComponents will register themselves with this system, and will
 * then be able to react to game events until unregistering themselves.
 *
 * @note Unlike other systems, the EventSystem handles the registering of
 * the ObserverComponent itself rather than using a ComponentManager. The reason
 * for this is that the ComponentManager works at the level of "these entities have
 * these components", while the EventSystem is concerned instead with which components
 * to notify. Note that the separation of different EventSystems for different types
 * of events would not work well if we simply took every observer from the component
 * manager.
 */
class EventSystem : public ISystem
{
public:
	EventSystem();
	virtual ~EventSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);

	/**
	 * @brief Method to designate this EventSystem as the one to handle input
	 * @return true iff there is no current InputSystem.
	 */
	bool MakeInputSystem();

	/**
	 * @brief Method to undesignate this EventSystem as the one to handle input
	 *
	 * @note This method only does anything if this EventSystem is currently
	 * handling input. Otherwise the call is ignored.
	*/
	void UnmakeInputSystem();

	/**
	 * @brief Inform all registered observers of an event
	 */
	void Inform(const Event& event);

private:
	friend class ObserverComponent;

	/**
	 * @brief Registers an observer with the system. Returs true if there was no error
	 *
	 * @note Observers will be added multiple times if multiple calls to this
	 * function is made with the same observer! Debug builds will check for this
	 * and log a warning, but release builds will not perform any such checks.
	 */
	bool RegisterObserver(ObserverComponent& observer);

	/**
	 * @brief Unregisters an observer with the system
	 *
	 * @note Only removes the first instance of observer found in the system.
	 * If the observer has been registered multiple times, the observer won't be
	 * completely unregistered.
	 */
	void UnregisterObserver(ObserverComponent& observer);

private:
	// This system doesn't necessarily only handle input, but it's the only one
	// that will get input events
	static EventSystem* s_pInputSystem;
	friend void KeyCallback(GLFWwindow*, int, int, int, int);

	EventQueue m_EventQueue;
	std::vector<ObserverComponent*> m_pObservers;
};

