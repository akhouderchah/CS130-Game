#pragma once

//The code for the Bullet Physics was learned from the following Youtube series, and it shares some
//part of the code with the code showed in those videos.
//https://www.youtube.com/watch?v=T9R9RWLzBcs&list=PLj4I-yBchUboXrXmPh6iepqw2lbAlq7gt&index=3

#include "Base.h"
#include "IComponent.h"
#include "CollisionComponent.h" 
#include <vector>

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
	~PhysicsComponent();

	void Refresh();

	void Tick(deltaTime_t dt);
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
	CollisionComponent * m_pColission;

	static btDynamicsWorld * s_pWorld;
	static btDispatcher * s_pDispatcher; //collision algorithm between each shapes
	static btCollisionConfiguration * s_pCollisionConfig; //stuff to do with collisions
	static btBroadphaseInterface * s_pBroadphase; //way to store data, like in a grid
	static btConstraintSolver * s_pSolver; //How much force is applied and all that
	static std::vector<int> s_WorldObjectsID;

	void renderPlane(bulletObject *);
	void renderSphere(bulletObject *);
	void renderBox(bulletObject *);
	void renderCylinder(bulletObject *);
};