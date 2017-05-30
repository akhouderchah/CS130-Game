#pragma once

#include "Base.h"
#include "IComponent.h"
#include "btBulletDynamicsCommon.h"
#include <vector>

struct bulletObject
{
	bool hit; //true if object is hit, false otherwise
	btRigidBody *body;
	btVector3 rotationFlags;
	btVector3 movementFlags;
	btVector3 gravity;
	bulletObject(btRigidBody *b, btVector3 &rot, btVector3 &mov) :
		body(b), hit(false), rotationFlags(rot), movementFlags(mov), gravity(btVector3(0.0, -10.0, 0.0)) {}
};

class MovableComponent;
class DrawComponent;
class TransformComponent;

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
	void ImpulseLeft() { m_LeftMovement = !m_LeftMovement; }
	void ImpulseRight() { m_RightMovement = !m_RightMovement; }

	static void AddBodyToWorld(btRigidBody *b) { s_pWorld->addRigidBody(b); }
	static btDynamicsWorld * getWorld() { return s_pWorld; }
	static void togglePause() { s_Pause = !s_Pause; }

	glm::vec3  GetVelocity();

	//Body creation and manipulation functions
	void addPlane();
	void addBox(float mass, const btVector3 &additionToDimensions = btVector3(0, 0, 0));
	void addSphere(float mass, float additionToRadius = 0);
	void addCylinder(float mass, float additionToDiameter = 0, float additionToHeight = 0);

	void setRotation(btVector3 &rotFlags) { m_pBody->rotationFlags = rotFlags; }
	void setMovement(btVector3 &movFlags) { m_pBody->movementFlags = movFlags; }
	void setGravity(btVector3 &g) { m_pBody->gravity = g; }
	void setBounciness(float bounce) { m_pBody->body->setRestitution(bounce); }

	void updateHitboxPosition(glm::vec3 vec);
	void updateHitboxAngle(glm::quat q);
	void toggleHitboxView();

	bulletObject *getBodyStructure() { return m_pBody; }

private:
	static float s_ImpulseWaitTime;
	float m_ImpulseWait;
	MovableComponent* m_pMover;
	DrawComponent * m_pDraw;
	TransformComponent* m_pTransform;

	static btDynamicsWorld * s_pWorld;
	static btDispatcher * s_pDispatcher; //collision algorithm between each shapes
	static btCollisionConfiguration * s_pCollisionConfig; //stuff to do with collisions
	static btBroadphaseInterface * s_pBroadphase; //way to store data, like in a grid
	static btConstraintSolver * s_pSolver; //How much force is applied and all that

	void updatePlane(bulletObject *);
	void updateSphere(bulletObject *);
	void updateBox(bulletObject *);
	void updateCylinder(bulletObject *);

	bool m_LeftMovement;
	bool m_RightMovement;
	static bool s_Pause;

	bool m_IsCollisionViewEnabled;
	bulletObject * m_pBody;
	Entity m_HitboxEntity;
};
