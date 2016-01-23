#pragma once

#include "../system/ISystem.h"

class InputSystem : public ISystem
{
public:
	InputSystem();
	virtual ~InputSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);

private:

};

