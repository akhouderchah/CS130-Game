#pragma once

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
	bool ImpulseLeft();
	bool ImpulseRight();


	static void AddBodyToWorld(btRigidBody *b) { s_pWorld->addRigidBody(b); }
	static btDynamicsWorld * getWorld() { return s_pWorld; }
	static void togglePause() { s_Pause = !s_Pause; }

	glm::vec3  GetVelocity();

private:
	static float s_ImpulseWaitTime;
	float m_ImpulseWait;
	MovableComponent* m_pMover;
	CollisionComponent * m_pColission;

	static btDynamicsWorld * s_pWorld;
	static btDispatcher * s_pDispatcher; //collision algorithm between each shapes
	static btCollisionConfiguration * s_pCollisionConfig; //stuff to do with collisions
	static btBroadphaseInterface * s_pBroadphase; //way to store data, like in a grid
	static btConstraintSolver * s_pSolver; //How much force is applied and all that

	void updatePlane(bulletObject *);
	void updateSphere(bulletObject *, CollisionComponent *);
	void updateBox(bulletObject *, CollisionComponent *);
	void updateCylinder(bulletObject *);

	bool m_LeftMovement;
	bool m_RightMovement;
	static bool s_Pause;
};
