#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "DrawComponent.h"
#include "EntityManager.h"
#include "MaterialComponent.h"
#include <algorithm> 


float PhysicsComponent::s_ImpulseWaitTime = 0.0f;

btBroadphaseInterface * PhysicsComponent::s_pBroadphase = new btDbvtBroadphase();
btCollisionConfiguration * PhysicsComponent::s_pCollisionConfig = new btDefaultCollisionConfiguration();
btDispatcher * PhysicsComponent::s_pDispatcher = new btCollisionDispatcher(s_pCollisionConfig);
btConstraintSolver * PhysicsComponent::s_pSolver = new btSequentialImpulseConstraintSolver;
btDynamicsWorld * PhysicsComponent::s_pWorld = new btDiscreteDynamicsWorld(s_pDispatcher, s_pBroadphase, s_pSolver, s_pCollisionConfig);

bool PhysicsComponent::s_Pause = false;


PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_ImpulseWait(0.f), m_pMover(nullptr), 
	m_pDraw(nullptr), m_pTransform(nullptr)
	
{
	m_LeftMovement = false;
	m_RightMovement = false;
	s_Pause = false;

	m_IsCollisionViewEnabled = true;

	m_pBody = nullptr;
}


PhysicsComponent::~PhysicsComponent()
{

	if (m_pBody != nullptr)
	{
		s_pWorld->removeCollisionObject(m_pBody->body);
		btMotionState *motionState = m_pBody->body->getMotionState();
		btCollisionShape *shape = m_pBody->body->getCollisionShape();

		delete m_pBody->body;
		delete shape;
		delete motionState;
		delete m_pBody;
	}


	//NOTE: I am not sure if I need to delete static pointers, but these are the ones that might need deletion

	delete s_pDispatcher;
	delete s_pCollisionConfig;
	delete s_pSolver;
	delete s_pBroadphase;
	delete s_pWorld;
}

void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pDraw = EntityManager::GetComponent<DrawComponent>(m_Entity);
	m_pTransform = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

void PhysicsComponent::Tick(deltaTime_t dt)
{
	if (m_pBody != nullptr)
	{
		m_pBody->body->setLinearFactor(m_pBody->movementFlags);
		m_pBody->body->setAngularFactor(m_pBody->rotationFlags);
		m_pBody->body->setGravity(m_pBody->gravity);

		if (m_LeftMovement)
		{
			m_pBody->body->activate();
			m_pBody->body->setLinearVelocity(btVector3(-0.5, m_pBody->body->getLinearVelocity().y(), m_pBody->body->getLinearVelocity().z()));
		}

		if (m_RightMovement)
		{
			m_pBody->body->activate();
			m_pBody->body->setLinearVelocity(btVector3(0.5, m_pBody->body->getLinearVelocity().y(), m_pBody->body->getLinearVelocity().z()));
		}

		m_pBody->hit = false;

		if (s_Pause)
		{
			s_pWorld->stepSimulation(0);
		}
		else
		{
			s_pWorld->stepSimulation(dt);
		}
		
		if (m_pBody->body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			updatePlane(m_pBody);
		}
		else if (m_pBody->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
		{
			updateBox(m_pBody);
		}
		else if (m_pBody->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			updateSphere(m_pBody);
		}
	}
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

void PhysicsComponent::updatePlane(bulletObject *obj)
{
	btRigidBody *plane = obj->body;

	if (plane->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
	{
		btTransform transform;
		plane->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
	}
}

void PhysicsComponent::updateBox(bulletObject *obj)
{
	btRigidBody *box = obj->body;

	if (box->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
	{
		btTransform transform;
		box->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
		btQuaternion ori = transform.getRotation();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();
		glm::quat quatArray;
		for (int i = 0; i < 3; i++)
		{
			moveVec[i] = vec[i] - moveVec[i];
		}

		for (int i = 0; i < 4; i++)
		{
			quatArray[i] = ori[i];
		}

		m_pMover->Move(moveVec);
		m_pMover->SetOrientation(quatArray);
		updateHitboxPosition(moveVec);
		updateHitboxAngle(quatArray);
	}
}

void PhysicsComponent::updateSphere(bulletObject *obj)
{
	btRigidBody *sphere = obj->body;

	if (sphere->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
	{
		
		btTransform transform;
		sphere->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
		btQuaternion ori = transform.getRotation();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();
		glm::quat quatArray;
		for (int i = 0; i < 3; i++)
		{
			moveVec[i] = vec[i] - moveVec[i];
		}

		for (int i = 0; i < 4; i++)
		{
			quatArray[i] = ori[i];
		}

		m_pMover->Move(moveVec);
		m_pMover->SetOrientation(quatArray);
		updateHitboxPosition(moveVec);
		updateHitboxAngle(quatArray);
	}
}

void PhysicsComponent::updateCylinder(bulletObject *obj)
{
	btRigidBody *cylinder = obj->body;

	if (cylinder->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
	{
		btTransform transform;
		cylinder->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
		btQuaternion ori = transform.getRotation();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();
		glm::quat quatArray;
		for (int i = 0; i < 3; i++)
		{
			moveVec[i] = vec[i] - moveVec[i];
		}

		for (int i = 0; i < 4; i++)
		{
			quatArray[i] = ori[i];
		}

		m_pMover->Move(moveVec);
		m_pMover->SetOrientation(quatArray);
		//col->updateHitboxPosition(moveVec);
		//col->updateHitboxAngle(quatArray);
	}
}

glm::vec3 PhysicsComponent::GetVelocity()
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

	m_pBody = new bulletObject(body, btVector3(0, 0, 0), btVector3(0, 0, 0));


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

void PhysicsComponent::addSphere(float mass, float additionToRadius)
{
	float f[3] = { m_pTransform->GetScale()[0], m_pTransform->GetScale()[1], m_pTransform->GetScale()[2] };
	float radius = std::max(std::max(f[1], f[2]), f[3]) + additionToRadius;
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

	m_pBody = new bulletObject(body, btVector3(1, 1, 1), btVector3(1, 1, 1));

	PhysicsComponent::AddBodyToWorld(m_pBody->body);

	//toggleHitboxView();
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