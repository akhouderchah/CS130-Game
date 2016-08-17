#include "DrawSystem.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent(Entity entity) :
	IComponent(entity)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Refresh()
{
}

