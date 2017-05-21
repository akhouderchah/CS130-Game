#pragma once

#include "Base.h"
#include "IComponent.h"
#include "btBulletDynamicsCommon.h"

class MovableComponent;
class DrawComponent;
class TransformComponent;

#define ENABLE_ROTATION true

enum class HitboxShapeType : uint8_t { PLANE, BOX, SPHERE, CYLINDER };



struct bulletObject
{
	int id;
	bool hit; //true if object is hit, false otherwise
	btRigidBody *body;
	HitboxShapeType hitboxShapeType;
	bool enableRotation;
	bulletObject(btRigidBody *b, HitboxShapeType type, int i, bool er) : body(b), hitboxShapeType(type), id(i), hit(false), enableRotation(er) {}
};



class CollisionComponent : public IComponent
{
public:
	CollisionComponent(Entity entity);
	~CollisionComponent();

	void Refresh();

	void Tick(deltaTime_t dt);

	void EnableHitboxView();
	void toggleHitboxView(glm::vec3);
	
	//New Functions
	std::vector<bulletObject *> getBodyVector() {return m_pBodies;}
	void addPlane(float additionToX = .0, float additionToY = .0, float additionToZ = .0); //This adds an infinite plane body
	void addBox(float mass = .0, float additionTowidth = .0, float additionToheight = .0, float additionTodepth = .0, float additionToX = .0, float additionToY = .0, float additionToZ = .0, bool enableRotation =  false);
	void addSphere(float mass, float additionToRadius = 0, float additionToX = 0, float additionToY = 0, float additionToZ = 0, bool enableRotation = false);
	void addCylinder(float, float, float, float, float, float, bool enableRotation = false);
	unsigned int getTotalBodies() { return s_TotalBodies; }

private:
	DrawComponent * m_pDraw;
	MovableComponent* m_pMover;
	TransformComponent* m_pTransform;

	Entity entity;
	bool isCollisionViewEnabled;

	glm::vec3 m_OldPosition;
	glm::vec3 m_OldScale;

	//New variables
	std::vector<bulletObject *> m_pBodies; //just a vector to store all bodies

	static unsigned int s_LastBodyID;
	static unsigned int s_TotalBodies;
};
