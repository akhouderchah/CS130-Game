#pragma once

#include "IComponent.h"

class DrawSystem;

/**
 * @brief Component that gives an entity the ability to act as a camera
 *
 * The render system uses information from this component in order to
 * generate the final MVP matrix. Having this as a component makes it
 * easy to switch between cameras as needed at runtime.
 */
class CameraComponent : public IComponent
{
public:
	CameraComponent(Entity entity);
	~CameraComponent();

	void Refresh();

private:
	
};
