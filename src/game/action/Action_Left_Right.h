#pragma once

#include "IAction.h"
#include "EntityManager.h"



/**
 * @brief Action to cause the provided entity to move left or right depending on passed on parameter.
 *
 * Obviously relies on the entity containing a PhysicsComponent. Lacking this,
 * the Action will not do anything (@todo perhaps log the event?)
 */

enum MovementDirection {left, right};


class Action_Left_Right : public IAction
{
public:
	Action_Left_Right(Entity entity, int);
	virtual ~Action_Left_Right();

	virtual bool operator()();

private:
	Entity m_Entity;
	int m_type;
};

