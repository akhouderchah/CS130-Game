#pragma once

#include "IAction.h"
#include "EntityManager.h"
/**
 * @brief Action to cause the provided entity to jump.
 *
 * Obviously relies on the entity containing a PhysicsComponent. Lacking this,
 * the Action will not do anything (@todo perhaps log the event?)
 */
class Action_Jump : public IAction
{
public:
	Action_Jump(Entity entity, int);
	virtual ~Action_Jump();

	virtual bool operator()();

private:
	Entity m_Entity;
	int m_type;
};

