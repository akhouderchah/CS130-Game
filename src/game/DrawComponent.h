#pragma once

#include "../system/IComponent.h"

class DrawComponent : public IComponent
{
public:
	DrawComponent(){}
	~DrawComponent(){}

	virtual void Tick(deltaTime_t dt){ (void)dt; } 

private:
	//DrawComponent(const DrawComponent& that);
	//DrawComponent& operator=(const DrawComponent& that);

	// Things that a draw system should know about go here //
};

