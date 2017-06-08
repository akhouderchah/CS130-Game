#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "MovableComponent.h"


bool PhysicsSystem::s_IsWorldInitialized = false;

btBroadphaseInterface *PhysicsSystem::m_pBroadphase = nullptr;
btCollisionConfiguration *PhysicsSystem::m_pCollisionConfig = nullptr;
btDispatcher *PhysicsSystem::m_pDispatcher = nullptr;
btConstraintSolver *PhysicsSystem::m_pSolver = nullptr;
btDynamicsWorld *PhysicsSystem::m_pWorld = nullptr;

PhysicsSystem::PhysicsSystem() :
	m_pPhysicsComponents(EntityManager::GetAll<PhysicsComponent>())
{
	
}



bool PhysicsSystem::Initialize()
{
	if (!s_IsWorldInitialized)
	{
		m_pBroadphase = new btDbvtBroadphase();
		m_pCollisionConfig = new btDefaultCollisionConfiguration();
		m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfig);
		m_pSolver = new btSequentialImpulseConstraintSolver;
		m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfig);

		m_pWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	}
	
	return true;
}



void PhysicsSystem::Shutdown()
{
	for (size_t i = 1; i < m_pPhysicsComponents.size(); ++i)
	{
		m_pWorld->removeCollisionObject(m_pPhysicsComponents[i]->getBodyStructure()->body);
		btMotionState *motionState = m_pPhysicsComponents[i]->getBodyStructure()->body->getMotionState();
		btCollisionShape *shape = m_pPhysicsComponents[i]->getBodyStructure()->body->getCollisionShape();

		delete m_pPhysicsComponents[i]->getBodyStructure()->body;
		delete shape;
		delete motionState;
		delete m_pPhysicsComponents[i]->getBodyStructure();
	}

	delete m_pWorld;
	delete m_pSolver;
	delete m_pCollisionConfig;
	delete m_pDispatcher;
	delete m_pBroadphase;
}



void PhysicsSystem::Tick(deltaTime_t dt)
{
	if (GetGameState() == EGS_STARTED)
		
	{
		
		for (size_t i = 1; i < m_pPhysicsComponents.size(); ++i)
		{
			bulletObject *bodyStructure = m_pPhysicsComponents[i]->getBodyStructure();

			bodyStructure->hit = false;
			bodyStructure->body->setLinearFactor(bodyStructure->movementFlags);
			bodyStructure->body->setAngularFactor(bodyStructure->rotationFlags);
			bodyStructure->body->setGravity(bodyStructure->gravity);

			if (m_pPhysicsComponents[i]->GetLeftMovement())
			{
				bodyStructure->body->activate();
				bodyStructure->body->setLinearVelocity(btVector3(-0.5, bodyStructure->body->getLinearVelocity().y(), bodyStructure->body->getLinearVelocity().z()));
			}

			if (m_pPhysicsComponents[i]->GetRightMovement())
			{
				bodyStructure->body->activate();
				bodyStructure->body->setLinearVelocity(btVector3(0.5, bodyStructure->body->getLinearVelocity().y(), bodyStructure->body->getLinearVelocity().z()));
			}

			m_pWorld->stepSimulation(dt / 10.f);


			if (bodyStructure->body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
			{
				updatePlane(bodyStructure);
			}
			else if (bodyStructure->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
			{
				updateBox(bodyStructure, m_pPhysicsComponents[i]->GetEntity());
			}
			else if (bodyStructure->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
			{
				updateSphere(bodyStructure, m_pPhysicsComponents[i]->GetEntity());
			}
		}
	}
	
}



void PhysicsSystem::updatePlane(bulletObject *obj)
{
	btRigidBody *plane = obj->body;

	if (plane->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
	{
		btTransform transform;
		plane->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
	}
}



void PhysicsSystem::updateBox(bulletObject *obj, Entity currentPhysicsEntity)
{
	btRigidBody *box = obj->body;

	if (box->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
	{
		btTransform transform;
		box->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
		btQuaternion ori = transform.getRotation();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(currentPhysicsEntity)->GetPosition();
		glm::quat quatArray;
		for (int i = 0; i < 3; i++)
		{
			moveVec[i] = vec[i] - moveVec[i];
		}

		for (int i = 0; i < 4; i++)
		{
			quatArray[i] = ori[i];
		}

		EntityManager::GetComponent<MovableComponent>(currentPhysicsEntity)->Move(moveVec);
		EntityManager::GetComponent<MovableComponent>(currentPhysicsEntity)->SetOrientation(quatArray);
		EntityManager::GetComponent<PhysicsComponent>(currentPhysicsEntity)->updateHitboxPosition(moveVec);
		EntityManager::GetComponent<PhysicsComponent>(currentPhysicsEntity)->updateHitboxAngle(quatArray);
	}
}



void PhysicsSystem::updateSphere(bulletObject *obj, Entity currentPhysicsEntity)
{
	btRigidBody *sphere = obj->body;

	if (sphere->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
	{
		btTransform transform;
		sphere->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
		btQuaternion ori = transform.getRotation();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(currentPhysicsEntity)->GetPosition();
		glm::quat quatArray;
		for (int i = 0; i < 3; i++)
		{
			moveVec[i] = vec[i] - moveVec[i];
		}

		for (int i = 0; i < 4; i++)
		{
			quatArray[i] = ori[i];
		}

		EntityManager::GetComponent<MovableComponent>(currentPhysicsEntity)->Move(moveVec);
		EntityManager::GetComponent<MovableComponent>(currentPhysicsEntity)->SetOrientation(quatArray);
		EntityManager::GetComponent<PhysicsComponent>(currentPhysicsEntity)->updateHitboxPosition(moveVec);
		EntityManager::GetComponent<PhysicsComponent>(currentPhysicsEntity)->updateHitboxAngle(quatArray);
	}
}



void PhysicsSystem::updateCylinder(bulletObject *obj, Entity currentPhysicsEntity)
{
	btRigidBody *cylinder = obj->body;

	if (cylinder->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
	{
		btTransform transform;
		cylinder->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();
		btQuaternion ori = transform.getRotation();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(currentPhysicsEntity)->GetPosition();
		glm::quat quatArray;
		for (int i = 0; i < 3; i++)
		{
			moveVec[i] = vec[i] - moveVec[i];
		}

		for (int i = 0; i < 4; i++)
		{
			quatArray[i] = ori[i];
		}

		EntityManager::GetComponent<MovableComponent>(currentPhysicsEntity)->Move(moveVec);
		EntityManager::GetComponent<MovableComponent>(currentPhysicsEntity)->SetOrientation(quatArray);
		EntityManager::GetComponent<PhysicsComponent>(currentPhysicsEntity)->updateHitboxPosition(moveVec);
		EntityManager::GetComponent<PhysicsComponent>(currentPhysicsEntity)->updateHitboxAngle(quatArray);
	}
}
