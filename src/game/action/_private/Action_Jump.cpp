#include "Action_Jump.h"
#include "PhysicsComponent.h"
#include "SoundComponent.h"



Action_Jump::Action_Jump(Entity entity, int type) :
	m_Entity(entity)
{
	m_type = type;
}

Action_Jump::~Action_Jump()
{
}

bool Action_Jump::operator()()
{
	PhysicsComponent* pPhys = m_Entity.GetAs<PhysicsComponent>();
	SoundComponent* pSnd = m_Entity.GetAs<SoundComponent>();
	if(pPhys)
	{
		if (m_type == 0)
		{
			pPhys->Impulse();
			pSnd->PlaySound("wingsFlap");
		}
		else if (m_type == 1)
		{
			//pPhys->ImpulseRight();
			pSnd->PlaySound("wingsFlap");
		}
	}
	return false;
}
