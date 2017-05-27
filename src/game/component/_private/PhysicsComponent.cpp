#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_ImpulseWaitTime = 0.0f;

btBroadphaseInterface * PhysicsComponent::s_pBroadphase = new btDbvtBroadphase();
btCollisionConfiguration * PhysicsComponent::s_pCollisionConfig = new btDefaultCollisionConfiguration();
btDispatcher * PhysicsComponent::s_pDispatcher = new btCollisionDispatcher(s_pCollisionConfig);
btConstraintSolver * PhysicsComponent::s_pSolver = new btSequentialImpulseConstraintSolver;
btDynamicsWorld * PhysicsComponent::s_pWorld = new btDiscreteDynamicsWorld(s_pDispatcher, s_pBroadphase, s_pSolver, s_pCollisionConfig);

bool PhysicsComponent::s_Pause = false;


PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_ImpulseWait(0.f), m_pMover(nullptr),
	m_pColission(nullptr)
{

	m_LeftMovement = false;
	m_RightMovement = false;
	s_Pause = false;
}

PhysicsComponent::~PhysicsComponent()
{
	//NOTE: I am not sure if I need to delete static pointers, but these are the ones that might need deletion

	//delete s_pDispatcher;
	//delete s_pCollisionConfig;
	//delete s_pSolver;
	//delete s_pBroadphase;
	//delete s_pWorld;
}

void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pColission = EntityManager::GetComponent<CollisionComponent>(m_Entity);
}

void PhysicsComponent::Tick(deltaTime_t dt)
{
	if (m_pColission != nullptr && m_pColission->GetID() !=0)
	{
		bulletObject * pBody = m_pColission->getBodyStructure();

		pBody->body->setLinearFactor(pBody->movementFlags);
		pBody->body->setAngularFactor(pBody->rotationFlags);
		pBody->body->setGravity(pBody->gravity);

		if (m_LeftMovement)
		{
			pBody->body->activate();
			pBody->body->setLinearVelocity(btVector3(-0.5, pBody->body->getLinearVelocity().y(), pBody->body->getLinearVelocity().z()));
		}

		if (m_RightMovement)
		{
			pBody->body->activate();
			pBody->body->setLinearVelocity(btVector3(0.5, pBody->body->getLinearVelocity().y(), pBody->body->getLinearVelocity().z()));
		}

		pBody->hit = false;
		if (s_Pause)
		{
			s_pWorld->stepSimulation(0);
		}
		else
		{
			s_pWorld->stepSimulation(dt);
		}
		

		if (pBody->body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			updatePlane(pBody);
		}
		else if (pBody->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
		{
			updateBox(pBody, m_pColission);
		}
		else if (pBody->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			updateSphere(pBody, m_pColission);
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

	if (m_pColission != nullptr && m_pColission->GetID() != 0)
	{
		btVector3 btVelocity = m_pColission->getBodyStructure()->body->getLinearVelocity();
		m_pColission->getBodyStructure()->body->activate();
		m_pColission->getBodyStructure()->body->setLinearVelocity(btVector3(btVelocity.x(), 4.0, btVelocity.z()));
	}
	
	return true;
}

bool PhysicsComponent::ImpulseLeft()
{
	if (m_ImpulseWait > 0.f)
	{
		return false;
	}
	m_ImpulseWait = s_ImpulseWaitTime;

	m_LeftMovement = !m_LeftMovement;

	return true;
}

bool PhysicsComponent::ImpulseRight()
{

	if (m_ImpulseWait > 0.f)
	{
		return false;
	}
	m_ImpulseWait = s_ImpulseWaitTime;

	m_RightMovement = !m_RightMovement;

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

void PhysicsComponent::updateBox(bulletObject *obj, CollisionComponent *col)
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
		col->updateHitboxPosition(moveVec);
		col->updateHitboxAngle(quatArray);
	}
}

void PhysicsComponent::updateSphere(bulletObject *obj, CollisionComponent *col)
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
		col->updateHitboxPosition(moveVec);
		col->updateHitboxAngle(quatArray);
	}
}

void PhysicsComponent::updateCylinder(bulletObject *obj)
{
	btRigidBody *cylinder = obj->body;

	if (cylinder->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
	{
		btTransform transform;
		cylinder->getMotionState()->getWorldTransform(transform);

		//Note that this actually puts cylinder on Y axis, so if you need to rotate, do it here.

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
	if (m_pColission != nullptr && m_pColission->GetID() != 0)
	{
		btVector3 btVelocity = m_pColission->getBodyStructure()->body->getLinearVelocity();
		return glm::vec3(btVelocity[0], btVelocity[1], btVelocity[2]);
	}
	else
	{
		return glm::vec3(0, 0, 0);
	}
}
