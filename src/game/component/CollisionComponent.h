#pragma once

#include "Base.h"
#include "IComponent.h"
#include "btBulletDynamicsCommon.h"

class MovableComponent;
class DrawComponent;
class TransformComponent;

struct bulletObject
{
	bool hit; //true if object is hit, false otherwise
	btRigidBody *body;
	btVector3 rotationFlags;
	btVector3 movementFlags;
	btVector3 gravity;
	bulletObject(btRigidBody *b, btVector3 &rot, btVector3 &mov): 
		body(b), hit(false), rotationFlags(rot), movementFlags(mov), gravity(btVector3(0.0,-10.0, 0.0)) {}
};


/**
* @brief Component that stores Bullet Physics bodies required for PhysicsComponent
*
* When creating this entity, we are only required to call one of the appropriate functions for the desired shape
* 
*/

//---Here I will provide the interfaces for each shape functions and a brief description---//

//addPlane
//Explanation: Adds an infinite static plane at the indicated location
//Interface: float additionToX(Default is 0) , float additionToY(Default is 0), float additionToZ(Default is 0)

//---The following objects have these properties. mass, enableRotation, bounciness---//
//Mass: sets the mass of the object. If mass is 0, object becomes static.
//rotationFlags: This flags controls the degree of rotation the object can have, all the possible values
//				 can be seen in an enum called RotationFlags that is located above. If the letter of axis
//				 is in the flag, then object is allowed to rotate around that axis. For example XZ will
//				 restrict rotation around Y axis and allow rotation around XZ. To turn off all rotations,
//				 set the flag to NONE.
//bounciness: This is a float value that set the objects bounciness level

//addBox: 
//Explanation: Adds a box.
//Interface: float mass(No default value),additionToWidth, additionToHeight, additionToDepth(Default is 0. Adds to existing dimension of object),
//			  additionToX, additionToY, additionToZ(Default is 0. Adds to existing location of object)
	

//addSphere:
//Explanation: Adds a sphere.
//Interface: float mass(No default value), additionToX, additionToY, additionToZ(Default is 0. Adds to existing location of object)
//			 

//addCylinder: NOT YET TESTED!!!
//Explanation: Adds a cylinder. NOTE: I think the cylinder by default is created on its side(X axis goes trough the hole)
//Interface: float mass(No default value),
//			 float additionTodiameter(default is 0), float additionToHeight(default is 0), 
//			 float additionToX(default is 0), float additionToY(default is 0), float additionToZ(default is 0)
	

class CollisionComponent : public IComponent
{
public:
	CollisionComponent(Entity entity);
	~CollisionComponent();

	void Refresh();

	void toggleHitboxView();
	bulletObject *getBodyStructure() {return m_pBody;}
	void updateHitboxPosition(glm::vec3 vec);
	void updateHitboxAngle(glm::quat q);

	void addPlane(); 

	void addBox(float mass, const btVector3 &additionToDimensions = btVector3(0, 0, 0));

	void addSphere(float mass, float additionToRadius = 0);

	void addCylinder(float mass, float additionToDiameter = 0, float additionToHeight = 0);

	void setRotation(btVector3 &rotFlags) { m_pBody->rotationFlags = rotFlags; }
	void setMovement(btVector3 &movFlags) { m_pBody->movementFlags = movFlags; }
	void setGravity(btVector3 &g) { m_pBody->gravity = g; }
	void setBounciness(float bounce) { m_pBody->body->setRestitution(bounce); }
	

private:
	DrawComponent * m_pDraw;
	MovableComponent* m_pMover;
	TransformComponent* m_pTransform;
	Entity entity;
	bool isCollisionViewEnabled;
	bulletObject * m_pBody;
};
