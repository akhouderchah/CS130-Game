#pragma once

#include "IComponent.h"

class DrawSystem;

class CameraComponent : public IComponent
{
public:
	CameraComponent(Entity entity);
	~CameraComponent();

	void Refresh();

private:
	
};
