#include "Action_ToggleHitboxView.h"
#include "CollisionComponent.h"

Action_ToggleHitboxView::Action_ToggleHitboxView()
{
}

Action_ToggleHitboxView::~Action_ToggleHitboxView()
{
}

bool Action_ToggleHitboxView::operator()()
{
	ConstVector<CollisionComponent*> pCol = EntityManager::GetAll<CollisionComponent>();

	for (unsigned int i = 1; i < pCol.size(); i++)
	{
		pCol[i]->toggleHitboxView();
	}

	return false;
}
