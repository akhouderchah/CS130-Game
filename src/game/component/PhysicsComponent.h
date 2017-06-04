#pragma once

#include "Base.h"
#include "IComponent.h"
#include "InputConstants.h"
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

	void Tick(deltaTime_t dt,EGameState State);
	bool Impulse();  // Returns true only if the impulse was successful

	void SetVelocity(glm::vec3 velocity){ m_Velocity = velocity; }
	void IncrementVelocity(glm::vec3 velocity){ m_Velocity += velocity; }

	void SetGravity(bool on){ m_IsGravityOn = on; }
	const glm::vec3 GetVelocity() const { return m_Velocity; }

private:
	static float s_Gravity;
	static float s_TerminalSpeed;
	static float s_ImpulseSpeed;
	static float s_ImpulseWaitTime;

	bool m_IsGravityOn;
	glm::vec3 m_Velocity;
	float m_ImpulseWait;
	MovableComponent* m_pMover;
};
