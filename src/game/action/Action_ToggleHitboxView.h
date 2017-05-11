#pragma once

#include "IAction.h"
#include "EntityManager.h"


class Action_ToggleHitboxView : public IAction
{
public:
	Action_ToggleHitboxView();
	virtual ~Action_ToggleHitboxView();

	virtual bool operator()();

private:
};

