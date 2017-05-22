#pragma once

#include "Base.h"
#include "IComponent.h"
#include "btBulletDynamicsCommon.h"

class MovableComponent;
class DrawComponent;
class TransformComponent;

#define ENABLE_ROTATION true

struct bulletObject
{
	bool hit; //true if object is hit, false otherwise
	btRigidBody *body;
	bool enableRotation;
	bulletObject(btRigidBody *b, bool er) : body(b), hit(false), enableRotation(er) {}
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
//enableRotation: If this flag is set, objects will be enable to rotate(Turn off if solid movements are desired)
//bounciness: This is a float value that set the objects bounciness level

//addBox: 
//Explanation: Adds a box.
//Interface: float mass(No default value), bool enableRotation(Default is false), float bounciness(Default is 0),
//			 additionToWidth, additionToHeight, additionToDepth(Default is 0. Adds to existing dimension of object),
//			 additionToX, additionToY, additionToZ(Default is 0. Adds to existing location of object)

//addSphere:
//Explanation: Adds a sphere.
//Interface: float mass(No default value), bool enableRotation(Default is false), float bounciness(Default is 0),
//			 additionToX, additionToY, additionToZ(Default is 0. Adds to existing location of object)

//addCylinder:
//Explanation: Adds a cylinder. NOTE: I think the cylinder by default is created on its side(X axis goes trough the hole)
//Interface: float mass(No default value), bool enableRotation(Default is false), float bounciness(Default is 0),
//			 float additionTodiameter(default is 0), float additionToHeight(default is 0), 
//			 float additionToX(default is 0), float additionToY(default is 0), float additionToZ(default is 0)
	

class CollisionComponent : public IComponent
{
public:
	CollisionComponent(Entity entity);
	~CollisionComponent();

	void Refresh();

	void Tick(deltaTime_t dt);


	void toggleHitboxView();
	bulletObject *getBodyStructure() {return m_pBody;}
	void updateHitboxPosition(glm::vec3 vec);

	void addPlane(float additionToX = .0, float additionToY = .0, float additionToZ = .0); //This adds an infinite plane body

	void addBox(float mass, bool enableRotation = false, float bounciness = 0, float additionTowidth = .0, 
		float additionToheight = .0, float additionTodepth = .0, float additionToX = .0, float additionToY = .0, float additionToZ = .0);

	void addSphere(float mass, bool enableRotation = false, float bounciness = 0, float additionToRadius = 0, 
		float additionToX = 0, float additionToY = 0, float additionToZ = 0);

	void addCylinder(float mass, float enableRotation, float bounciness, float additionTodiameter, float additionToHeight, 
		float additionToX = 0, float additionToY = 0, float additionToZ = 0);

private:
	DrawComponent * m_pDraw;
	MovableComponent* m_pMover;
	TransformComponent* m_pTransform;

	Entity entity;
	bool isCollisionViewEnabled;

	glm::vec3 m_OldPosition;
	glm::vec3 m_OldScale;

	bulletObject * m_pBody;
};
