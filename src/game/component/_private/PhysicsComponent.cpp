#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "DrawComponent.h"
#include "EntityManager.h"
#include "MaterialComponent.h"
#include <algorithm> 
#include "PhysicsSystem.h"


float PhysicsComponent::s_ImpulseWaitTime = 0.0f;


PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_ImpulseWait(0.f), m_pMover(nullptr), 
	m_pDraw(nullptr), m_pTransform(nullptr)
{

	m_LeftMovement = false;
	m_RightMovement = false;

	m_IsCollisionViewEnabled = true;

	m_pBody = nullptr;
}



void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pDraw = EntityManager::GetComponent<DrawComponent>(m_Entity);
	m_pTransform = EntityManager::GetComponent<TransformComponent>(m_Entity);
}



bool PhysicsComponent::Impulse()
{
	if(m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;

	if (m_pBody != nullptr)
	{
		btVector3 btVelocity = m_pBody->body->getLinearVelocity();
		m_pBody->body->activate();
		m_pBody->body->setLinearVelocity(btVector3(btVelocity.x(), 4.0, btVelocity.z()));
	}
	return true;
}



glm::vec3 PhysicsComponent::GetVelocity() const
{
	if (m_pBody != nullptr)
	{
		btVector3 btVelocity = m_pBody->body->getLinearVelocity();
		return glm::vec3(btVelocity[0], btVelocity[1], btVelocity[2]);
	}
	else
	{
		return glm::vec3(0, 0, 0);
	}
}



void PhysicsComponent::addPlane()
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

	m_pBody = new bulletObject(body, btVector3(0, 0, 0), btVector3(0, 0, 0), btVector3(0.0, -10.0, 0.0));

	m_HitboxEntity = EntityManager::CreateEntity();
	m_HitboxEntity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z),
		glm::vec3(m_pTransform->GetScale()[0], 0.01f, m_pTransform->GetScale()[2]));
	m_HitboxEntity.Add<MovableComponent>();
	m_HitboxEntity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = m_HitboxEntity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);

	toggleHitboxView();

	PhysicsSystem::GetWorld()->addRigidBody(m_pBody->body);
}



void PhysicsComponent::addBox(float mass, const btVector3 &additionToDimensions)
{
	float X = m_pTransform->GetPosition()[0];
	float Y = m_pTransform->GetPosition()[1];
	float Z = m_pTransform->GetPosition()[2];
	float width = m_pTransform->GetScale()[0] + additionToDimensions.x();
	float height = m_pTransform->GetScale()[1] + additionToDimensions.y();
	float depth = m_pTransform->GetScale()[2] + additionToDimensions.z();

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(X, Y, Z));

	btBoxShape *box = new btBoxShape(btVector3(width, height, depth));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		box->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(0);

	m_pBody = new bulletObject(body, btVector3(0, 0, 0), btVector3(0, 0, 0), btVector3(0.0, -10.0, 0.0));

	m_HitboxEntity = EntityManager::CreateEntity();
	m_HitboxEntity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), glm::vec3(width, height, depth));
	m_HitboxEntity.Add<MovableComponent>();
	m_HitboxEntity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = m_HitboxEntity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);

	toggleHitboxView();

	PhysicsSystem::GetWorld()->addRigidBody(m_pBody->body);
}



void PhysicsComponent::addSphere(float mass, float additionToRadius)
{
	float f[3] = { m_pTransform->GetScale()[0], m_pTransform->GetScale()[1], m_pTransform->GetScale()[2] };
	float radius = std::max(std::max(f[0], f[1]), f[2]) + additionToRadius;
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

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(0);

	m_pBody = new bulletObject(body, btVector3(0, 0, 0), btVector3(0, 0, 0), btVector3(0.0, -10.0, 0.0));

	m_HitboxEntity = EntityManager::CreateEntity();
	m_HitboxEntity.Add<TransformComponent>()->Init(glm::vec3(X, Y, Z), glm::vec3(radius, radius, radius));
	m_HitboxEntity.Add<MovableComponent>();
	m_HitboxEntity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = m_HitboxEntity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "circle.tga", TextureType::RGBA);

	toggleHitboxView();

	PhysicsSystem::GetWorld()->addRigidBody(m_pBody->body);
}



void PhysicsComponent::addCylinder(float mass, float additionTodiameter, float additionToHeight)
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

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
	btRigidBody *body = new btRigidBody(info);

	body->setRestitution(0);

	m_pBody = new bulletObject(body, btVector3(0, 0, 0), btVector3(0, 0, 0), btVector3(0.0, -10.0, 0.0));

	toggleHitboxView();

	PhysicsSystem::GetWorld()->addRigidBody(m_pBody->body);
}



void PhysicsComponent::toggleHitboxView()
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



void PhysicsComponent::updateHitboxPosition(glm::vec3 vec)
{
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(m_HitboxEntity);
	move->Move(vec);
}


void PhysicsComponent::updateHitboxAngle(glm::quat q)
{
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(m_HitboxEntity);
	move->SetOrientation(q);
}