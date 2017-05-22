#pragma once

#include "Base.h"
#include "EntityManager.h"
#include "InputConstants.h"

/**
 * @brief Interface for game actions.
 *
 * An action is the method used to react to game events, whether those
 * events are generated internally (e.g. achievements) or externally
 * (e.g. user input). Actions are linked to Events by the ObserverComponent.
 * @see ObserverComponent
 *
 * @note Implemented as a functor. Thus, child classes should be
 * modifying the operator() to override functionality.
 *
 * @note Although the entity system allows components of the same entity
 * to reference each other with pointers (through the use of the Refresh()
 * method), Actions cannot reference components directly with pointers, and
 * therefore must go through the entity system every invocation in order to access
 * an entity's components. Since this is slower, actions should not be used to
 * perform something that will happen often (on the scale of many times a second).
 */
class IAction
{
public:
	IAction(){}
	virtual ~IAction(){}

	virtual bool operator()() = 0;
};

