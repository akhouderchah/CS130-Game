#pragma once

#include "IComponent.h"

class DrawSystem;

class CameraComponent : public IComponent
{
public:
	CameraComponent();
	~CameraComponent();

	void Destroy();

private:
	
};
