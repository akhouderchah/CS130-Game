#include "Action_Left_Right.h"
#include "PhysicsComponent.h"
#include "SoundComponent.h"


Action_Left_Right::Action_Left_Right(Entity entity, int type) :
	m_Entity(entity)
{
	m_type = type;
}

Action_Left_Right::~Action_Left_Right()
{
}

bool Action_Left_Right::operator()()
{
	PhysicsComponent* pPhys = m_Entity.GetAs<PhysicsComponent>();
	if(pPhys)
	{
		if (m_type == 0)
		{
			pPhys->ImpulseLeft();
		}
		else if (m_type == 1)
		{
			pPhys->ImpulseRight();
		}
	}
	return false;
}
