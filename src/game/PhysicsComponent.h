#pragma once

#include "IComponent.h"
#include "Base.h"

class MovableComponent;

/**
 * @brief Component to give physical simulation capabilities.
 *
 * Requires the MovableComponent to function properly.
 */
class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(MovableComponent& mover);

	void Tick(deltaTime_t dt);
	bool Impulse();  // Returns true only if the impulse was successful
	void Destroy(){}
	
private:
	static float s_Gravity;
	static float s_TerminalSpeed;

	glm::vec3 m_Velocity;
	MovableComponent* m_pMover;
};
