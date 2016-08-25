#include "Action_Jump.h"
#include "PhysicsComponent.h"

Action_Jump::Action_Jump(Entity entity) :
	m_Entity(entity)
{
}

Action_Jump::~Action_Jump()
{
}

bool Action_Jump::operator()()
{
	PhysicsComponent* pPhys = m_Entity.GetAs<PhysicsComponent>();
	if(pPhys)
	{
		pPhys->Impulse();
	}
	return false;
}
