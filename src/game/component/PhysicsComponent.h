#pragma once

#include "Base.h"
#include "IComponent.h"

class MovableComponent;

/**
 * @brief Component to give physical simulation capabilities.
 *
 * Requires the MovableComponent to function properly.
 */
class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(Entity entity);

	void Refresh();

	void Tick(deltaTime_t dt);
	bool Impulse();  // Returns true only if the impulse was successful

private:
	static float s_Gravity;
	static float s_TerminalSpeed;
	static float s_ImpulseSpeed;
	static float s_ImpulseWaitTime;

	glm::vec3 m_Velocity;
	float m_ImpulseWait;
	MovableComponent* m_pMover;
};
