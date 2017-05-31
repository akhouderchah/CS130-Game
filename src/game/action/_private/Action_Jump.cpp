#include "Action_Jump.h"
#include "PhysicsComponent.h"
#include "SoundComponent.h"



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
	SoundComponent* pSnd = m_Entity.GetAs<SoundComponent>();
	if(pPhys)
	{

		pPhys->Impulse();
		pSnd->PlaySound("wingsFlap");

	}
	return false;
}
