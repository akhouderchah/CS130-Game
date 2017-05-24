#include "MovableComponent.h"
#include "CollisionComponent.h"
#include "DrawComponent.h"
#include "EntityManager.h"
#include "MaterialComponent.h"
#include <algorithm> 


CollisionComponent::CollisionComponent(Entity entity) :
	IComponent(entity), m_pMover(nullptr), m_pDraw(nullptr),
	m_pTransform(nullptr)
{
	isCollisionViewEnabled = true;
}

CollisionComponent::~CollisionComponent()
{
	/*
	for (int i = 0; i < m_pBodies.size(); i++)
	{
	world->removeCollisionObject(m_pBodies[i]->body);
	btMotionState *motionState = m_pBodies[i]->body->getMotionState();
	btCollisionShape *shape = m_pBodies[i]->body->getCollisionShape();

	delete m_pBodies[i]->body;
	delete shape;
	delete motionState;
	delete m_pBodies[i];
	}
	*/
}

void CollisionComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pDraw = EntityManager::GetComponent<DrawComponent>(m_Entity);
	m_pTransform = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

void CollisionComponent::Tick(deltaTime_t dt)
{
	//@TODO This have a huge performance hit, need to find a better way to do this...
	//glm::vec3 deltaPosition = m_pTransform->GetPosition() - m_OldPosition;
	//glm::vec3 deltaScale = m_pTransform->GetScale() - m_OldScale;

	//m_OldPosition = m_pTransform->GetPosition();
	//m_OldScale = m_pTransform->GetScale();

	//EntityManager::GetComponent<MovableComponent>(entity)->Move(deltaPosition);
	//EntityManager::GetComponent<MovableComponent>(entity)->Scale(deltaScale);
}


void CollisionComponent::toggleHitboxView()
{
	MovableComponent * pMove = EntityManager::GetComponent<MovableComponent>(entity);
	
	if (pMove)
	{
		glm::vec3 vec;
		if (isCollisionViewEnabled == true)
		{
			vec = { 100, 100, 100 };
			isCollisionViewEnabled = false;
		}
		else
		{
			vec = { -100, -100, -100 };
			isCollisionViewEnabled = true;
		}
		pMove->Move(vec);
	}
}

void CollisionComponent::updateHitboxPosition(glm::vec3 vec)
{
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(entity);
	move->Move(vec);
}


void CollisionComponent::updateHitboxAngle(glm::quat q)
{
	//MovableComponent * move = EntityManager::GetComponent<MovableComponent>(entity);
	//move->SetOrientation(q);
}


void CollisionComponent::addPlane(float additionToX, float additionToY, float additionToZ)
{
	float X = m_pTransform->GetPosition()[0] + additionToX;
	float Y = m_pTransform->GetPosition()[1] + additionToY;
	float Z = m_pTransform->GetPosition()[2] + additionToZ;

	btTransform transform;
	transform.setIdentity();

	transform.setOrigin(btVector3(X, Y, Z));

	btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);

	btMotionState *motion = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(1); //Bounciness of the object

	m_pBody = new bulletObject(body, NONE, NONE);


	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), 
		glm::vec3(m_pTransform->GetScale()[0], 0.01f, m_pTransform->GetScale()[2]));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);
}


void CollisionComponent::addBox(float mass, float additionTowidth, float additionToheight, float additionTodepth, 
	float additionToX, float additionToY, float additionToZ)
	
{
	float X = m_pTransform->GetPosition()[0] + additionToX;
	float Y = m_pTransform->GetPosition()[1] + additionToY;
	float Z = m_pTransform->GetPosition()[2] + additionToZ;
	float width = m_pTransform->GetScale()[0] + additionTowidth;
	float height = m_pTransform->GetScale()[1] + additionToheight;
	float depth = m_pTransform->GetScale()[2] + additionTodepth;


	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(X,Y, Z));

	btBoxShape *box = new btBoxShape(btVector3(width, height, depth));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		box->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);
	
	m_pBody = new bulletObject(body, XYZ, XYZ);

	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), glm::vec3(width, height, depth));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);
}

void CollisionComponent::addSphere(float mass, float additionToRadius, float additionToX, float additionToY, float additionToZ)
{
	float f[3] = { m_pTransform->GetScale()[0], m_pTransform->GetScale()[1], m_pTransform->GetScale()[2] };
	float radius = std::max(std::max(f[1],f[2]), f[3]) + additionToRadius;
	float X = m_pTransform->GetPosition()[0] + additionToX;
	float Y = m_pTransform->GetPosition()[1] + additionToY;
	float Z = m_pTransform->GetPosition()[2] + additionToZ;

	btTransform transform;
	transform.setIdentity();

	transform.setOrigin(btVector3(X, Y, Z));

	btSphereShape *sphere = new btSphereShape(radius);

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		sphere->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);

	m_pBody = new bulletObject(body, XYZ, XYZ);

	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), glm::vec3(radius, radius, radius));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "circle.tga", TextureType::RGBA);
}


void CollisionComponent::addCylinder(float mass, float additionTodiameter, float additionToHeight, float additionToX, float additionToY, float additionToZ)
{
	float X = m_pTransform->GetPosition()[0] + additionToX;
	float Y = m_pTransform->GetPosition()[1] + additionToY;
	float Z = m_pTransform->GetPosition()[2] + additionToZ;
	float diameter = m_pTransform->GetScale()[0] + additionTodiameter;
	float height = m_pTransform->GetScale()[1] + additionToHeight;

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(X, Y, Z));

	btCylinderShape *cylinder = new btCylinderShape(btVector3(diameter, height, diameter));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		cylinder->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
	btRigidBody *body = new btRigidBody(info);

	m_pBody = new bulletObject(body, XYZ, XYZ);
}
