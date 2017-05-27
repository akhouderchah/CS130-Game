#include "MovableComponent.h"
#include "CollisionComponent.h"
#include "DrawComponent.h"
#include "EntityManager.h"
#include "MaterialComponent.h"
#include <algorithm> 
#include "PhysicsComponent.h"


CollisionComponent::CollisionComponent(Entity entity) :
	IComponent(entity), m_pMover(nullptr), m_pDraw(nullptr),
	m_pTransform(nullptr)
{
	m_IsCollisionViewEnabled = true;
}

CollisionComponent::~CollisionComponent()
{
	if (EntityManager::GetComponent<CollisionComponent>(m_Entity)->GetID() != 0)
	{
		PhysicsComponent::getWorld()->removeCollisionObject(m_pBody->body);
		btMotionState *motionState = m_pBody->body->getMotionState();
		btCollisionShape *shape = m_pBody->body->getCollisionShape();

		delete m_pBody->body;
		delete shape;
		delete motionState;
		delete m_pBody;
	}
}

void CollisionComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pDraw = EntityManager::GetComponent<DrawComponent>(m_Entity);
	m_pTransform = EntityManager::GetComponent<TransformComponent>(m_Entity);
}


void CollisionComponent::toggleHitboxView()
{
	MovableComponent * pMove = EntityManager::GetComponent<MovableComponent>(m_HitboxEntity);
	
	if (pMove)
	{
		glm::vec3 vec;
		if (m_IsCollisionViewEnabled == true)
		{
			vec = { 100, 100, 100 };
			m_IsCollisionViewEnabled = false;
		}
		else
		{
			vec = { -100, -100, -100 };
			m_IsCollisionViewEnabled = true;
		}
		pMove->Move(vec);
	}
}

void CollisionComponent::updateHitboxPosition(glm::vec3 vec)
{
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(m_HitboxEntity);
	move->Move(vec);
}


void CollisionComponent::updateHitboxAngle(glm::quat q)
{
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(m_HitboxEntity);
	move->SetOrientation(q);
}


void CollisionComponent::addPlane()
{
	float X = m_pTransform->GetPosition()[0];
	float Y = m_pTransform->GetPosition()[1];
	float Z = m_pTransform->GetPosition()[2];

	btTransform transform;
	transform.setIdentity();

	transform.setOrigin(btVector3(X, Y, Z));

	btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);

	btMotionState *motion = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(1);

	m_pBody = new bulletObject(body, btVector3(0,0,0), btVector3(0, 0, 0));
	

	m_HitboxEntity = EntityManager::CreateEntity();
	m_HitboxEntity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), 
		glm::vec3(m_pTransform->GetScale()[0], 0.01f, m_pTransform->GetScale()[2]));
	m_HitboxEntity.Add<MovableComponent>();
	m_HitboxEntity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = m_HitboxEntity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);

	PhysicsComponent::AddBodyToWorld(m_pBody->body);

	//toggleHitboxView();
}


void CollisionComponent::addBox(float mass, const btVector3 &additionToDimensions)
{
	float X = m_pTransform->GetPosition()[0];
	float Y = m_pTransform->GetPosition()[1];
	float Z = m_pTransform->GetPosition()[2];
	float width = m_pTransform->GetScale()[0] + additionToDimensions.x();
	float height = m_pTransform->GetScale()[1] + additionToDimensions.y();
	float depth = m_pTransform->GetScale()[2] + additionToDimensions.z();

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

	float tempMass = (EntityManager::GetComponent<PhysicsComponent>(m_Entity)->GetID() == 0) ? 0 : mass;

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(0);
	
	m_pBody = new bulletObject(body, btVector3(1, 1, 1), btVector3(1, 1, 1));

	m_HitboxEntity = EntityManager::CreateEntity();
	m_HitboxEntity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), glm::vec3(width, height, depth));
	m_HitboxEntity.Add<MovableComponent>();
	m_HitboxEntity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = m_HitboxEntity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);

	PhysicsComponent::AddBodyToWorld(m_pBody->body);

	//toggleHitboxView();
}

void CollisionComponent::addSphere(float mass, float additionToRadius)
{
	float f[3] = { m_pTransform->GetScale()[0], m_pTransform->GetScale()[1], m_pTransform->GetScale()[2] };
	float radius = std::max(std::max(f[1],f[2]), f[3]) + additionToRadius;
	float X = m_pTransform->GetPosition()[0];
	float Y = m_pTransform->GetPosition()[1];
	float Z = m_pTransform->GetPosition()[2];

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

	float tempMass = (EntityManager::GetComponent<PhysicsComponent>(m_Entity)->GetID () == 0) ? 0 : mass;

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(0);

	m_pBody = new bulletObject(body, btVector3(1, 1, 1), btVector3(1, 1, 1));

	m_HitboxEntity = EntityManager::CreateEntity();
	m_HitboxEntity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), glm::vec3(radius, radius, radius));
	m_HitboxEntity.Add<MovableComponent>();
	m_HitboxEntity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = m_HitboxEntity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "circle.tga", TextureType::RGBA);

	PhysicsComponent::AddBodyToWorld(m_pBody->body);

	//toggleHitboxView();
}


void CollisionComponent::addCylinder(float mass, float additionTodiameter, float additionToHeight)
{
	float X = m_pTransform->GetPosition()[0];
	float Y = m_pTransform->GetPosition()[1];
	float Z = m_pTransform->GetPosition()[2];
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

	float tempMass = (EntityManager::GetComponent<PhysicsComponent>(m_Entity)->GetID() == 0) ? 0 : mass;

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(0);

	m_pBody = new bulletObject(body, btVector3(1, 1, 1), btVector3(1, 1, 1));

	PhysicsComponent::AddBodyToWorld(m_pBody->body);

	//toggleHitboxView();
}
