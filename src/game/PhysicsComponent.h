#pragma once

#include "IComponent.h"
#include "Base.h"

class MovableComponent;

class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(MovableComponent& mover);

	void Tick(deltaTime_t dt);
	bool Impulse();  // Returns true only if the impulse was successful
	void Destroy(){}
	
private:
	static float s_Gravity;

	glm::vec3 m_Velocity;
	MovableComponent* m_pMover;
};
