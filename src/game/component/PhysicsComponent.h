#pragma once

#include "Base.h"
#include "IComponent.h"
#include "btBulletDynamicsCommon.h"
#include <vector>

struct bulletObject
{
	bool hit;
	btRigidBody *body;
	btVector3 rotationFlags;
	btVector3 movementFlags;
	btVector3 gravity;
	bulletObject(btRigidBody *b, const btVector3 &rot, const btVector3 &mov, const btVector3 &grav) :
		hit(false), body(b), rotationFlags(rot), movementFlags(mov), gravity(grav) {}
};

class MovableComponent;
class DrawComponent;
class TransformComponent;

/**
* @brief Component to give physical simulation capabilities.
*
* Requires addition of rigid body in order to function properly
*/
class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(Entity entity);

	void Refresh();

	bool Impulse();
	void ImpulseLeft() { m_LeftMovement = !m_LeftMovement; }
	void ImpulseRight() { m_RightMovement = !m_RightMovement; }

	bool GetLeftMovement() const { return m_LeftMovement;  }
	bool GetRightMovement() const { return m_RightMovement; }

	/**
	* @brief Adds an infinite static plane at the location provided by the TransformComponent position
	*/
	void addPlane();

	/**
	* @brief Adds a box at the location provided by the TransformComponent position
	*
	* Interface: float mass(No default value), btVector3 &additionToDimensions(Default value (0,0,0))
	*/
	void addBox(float mass, const btVector3 &additionToDimensions = btVector3(0, 0, 0));

	/**
	* @brief Adds a sphere at the location provided by the TransformComponent position
	*
	* Interface: float mass(No default value), float additionToRadius(Default value 0)
	*/
	void addSphere(float mass, float additionToRadius = 0);

	/**
	* @brief Adds a cylinder at a location provided by the TransformComponent position
	*
	* NOTE: Not yet tested, need to load 3d shapes to perform proper testing.
	*
	* Interface: float mass(No default value), float additionToDiameter (Default value 0), float additionToHeight (Default value 0)
	*/
	void addCylinder(float mass, float additionToDiameter = 0, float additionToHeight = 0);

	/**
	* @brief Allows or restricts rotation on a given axis.
	*
	* Value of 1 allows rotation, value of 0 restricts rotation. By default bodies have allowed rotation on all 3 axes
	*
	* Interface: btVector &rotFlags(No default value)
	*/
	void setRotation(const btVector3 &rotFlags) { m_pBody->rotationFlags = rotFlags; }

	/**
	* @brief Allows or restricts movement on a given axis.
	*
	* Value of 1 allows movement, value of 0 restricts movement. By default bodies have allowed movement on all 3 axes
	*
	* Interface: btVector &movFlags(No default value)
	*/
	void setMovement(const btVector3 &movFlags) { m_pBody->movementFlags = movFlags; }

	/**
	*
	* @brief Allows to set the gravity for a given body. By default gravity is set to (0, -10, 0)
	*
	* Interface: btVector3 &g(No default value)
	*/
	void setGravity(const btVector3 &g) { m_pBody->gravity = g; }

	/**
	* @brief Allows to set the bounce property of the given object. By default, bounciness is set to  1 for plane and to 0 for everything else
	*
	* Interface: float bounce(No default value)
	*/
	void setBounciness(float bounce) { m_pBody->body->setRestitution(bounce); }

	/**
	* @brief This is an Aux function for the temporary solution to display hitboxes.
	* Interface: glm::vec3 vec(No default value)
	*/
	void updateHitboxPosition(glm::vec3 vec);

	/**
	* @brief This is an Aux function for the temporary solution to display hitbnxes.
	* Interface: glm::quat q(No default value)
	*/
	void updateHitboxAngle(glm::quat q);

	/**
	* @brief This is an Aux function for the temporary solution to toggle hitbox view.
	* Interface: None
	*/
	void toggleHitboxView();

	/**
	* @brief Retruns a pointer to physical body structure.
	* Interface: None
	*/
	bulletObject *getBodyStructure() const { return m_pBody; }

	/**
	* @brief Retruns a velocity vector of a body.
	* Interface: None
	*/
	glm::vec3  GetVelocity() const;

private:
	static float s_ImpulseWaitTime;
	float m_ImpulseWait;
	MovableComponent* m_pMover;
	DrawComponent * m_pDraw;
	TransformComponent* m_pTransform;

	bool m_LeftMovement;
	bool m_RightMovement;

	bool m_IsCollisionViewEnabled;
	bulletObject * m_pBody;
	Entity m_HitboxEntity;
};
