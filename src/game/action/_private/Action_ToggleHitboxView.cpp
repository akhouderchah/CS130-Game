#include "Action_ToggleHitboxView.h"
#include "PhysicsComponent.h"

Action_ToggleHitboxView::Action_ToggleHitboxView()
{
}

Action_ToggleHitboxView::~Action_ToggleHitboxView()
{
}

bool Action_ToggleHitboxView::operator()()
{
	ConstVector<PhysicsComponent*> pPhysics = EntityManager::GetAll<PhysicsComponent>();

	for (unsigned int i = 1; i < pPhysics.size(); i++)
	{
		pPhysics[i]->toggleHitboxView();
	}

	return false;
}
