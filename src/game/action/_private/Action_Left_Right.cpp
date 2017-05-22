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
	SoundComponent* pSnd = m_Entity.GetAs<SoundComponent>();
	if(pPhys)
	{
		if (m_type == 0)
		{
			pPhys->ImpulseLeft();
			pSnd->PlaySound("wingsFlap");
		}
		else if (m_type == 1)
		{
			pPhys->ImpulseRight();
			pSnd->PlaySound("wingsFlap");
		}
	}
	return false;
}
