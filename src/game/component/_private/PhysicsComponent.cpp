#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_ImpulseSpeed = 2.85f;
float PhysicsComponent::s_ImpulseWaitTime = 0.0f;

btDynamicsWorld * PhysicsComponent::s_pWorld = nullptr;
btDispatcher * PhysicsComponent::s_pDispatcher = nullptr;
btCollisionConfiguration * PhysicsComponent::s_pCollisionConfig = nullptr;
btBroadphaseInterface * PhysicsComponent::s_pBroadphase = nullptr;
btConstraintSolver * PhysicsComponent::s_pSolver = nullptr;
std::vector<int> PhysicsComponent::s_WorldCollisionComponentID;


PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_IsGravityOn(true), m_Velocity(0,0,0),
	m_ImpulseWait(0.f), m_pMover(nullptr),
	m_pColission(nullptr)
{
	//Initializing the world
	s_pBroadphase = new btDbvtBroadphase();
	s_pCollisionConfig = new btDefaultCollisionConfiguration();
	s_pDispatcher = new btCollisionDispatcher(s_pCollisionConfig);
	s_pSolver = new btSequentialImpulseConstraintSolver;
	s_pWorld = new btDiscreteDynamicsWorld(s_pDispatcher, s_pBroadphase, s_pSolver, s_pCollisionConfig);
	
	s_pWorld->setGravity(btVector3(0, -10, 0)); //Sets the gravity in the world
}

PhysicsComponent::~PhysicsComponent()
{
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
	bulletObject * pBody = m_pColission->getBodyStructure();

	//check to see if current body has already been added to the world
	if (std::find(s_WorldCollisionComponentID.begin(), s_WorldCollisionComponentID.end(), m_pColission->GetID()) == s_WorldCollisionComponentID.end())
	{
		s_pWorld->addRigidBody(pBody->body);
		s_WorldCollisionComponentID.push_back(m_pColission->GetID());
	}
	
	
	pBody->hit = false;

	s_pWorld->stepSimulation(dt); //FPS for the physics calculations in seconds

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

bool PhysicsComponent::Impulse()
{
	if(m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;


	m_pColission->getBodyStructure()->body->activate();
	m_pColission->getBodyStructure()->body->setLinearVelocity(btVector3(0.0, 5.0, 0.0));

	return true;
}

bool PhysicsComponent::ImpulseLeft()
{
	if (m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;

	bulletObject * bodies = m_pColission->getBodyStructure();

	bodies->body->activate();
	bodies->body->setLinearVelocity(btVector3(-2.0, 0.0, 0.0));
	return true;
}

bool PhysicsComponent::ImpulseRight()
{
	if (m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;

	bulletObject * bodies = m_pColission->getBodyStructure();

	bodies->body->activate();
	bodies->body->setLinearVelocity(btVector3(2.0, 0.0, 0.0));
	return true;
}



void PhysicsComponent::updatePlane(bulletObject *obj)
{
	btRigidBody *plane = obj->body;

	if (plane->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
	{
		btTransform transform;
		plane->getMotionState()->getWorldTransform(transform);
	}
}

void PhysicsComponent::updateBox(bulletObject *obj, CollisionComponent *col)
{
	btRigidBody *box = obj->body;

	if (box->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
	{
		if (!obj->enableRotation)
		{
			box->setInvInertiaDiagLocal(btVector3(0, 0, 0));
			box->updateInertiaTensor();
		}

		btTransform transform;
		box->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();

		moveVec[0] = -(moveVec[0] - vec[0]);
		moveVec[1] = -(moveVec[1] - vec[1]);
		moveVec[2] = -(moveVec[2] - vec[2]);

		btMatrix3x3 mat = transform.getBasis();

		m_pMover->Move(moveVec);
		col->updateHitboxPosition(moveVec);
	}
}


void PhysicsComponent::updateSphere(bulletObject *obj, CollisionComponent *col)
{
	btRigidBody *sphere = obj->body;

	if (sphere->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
	{
		if (!obj->enableRotation)
		{
			sphere->setInvInertiaDiagLocal(btVector3(0, 0, 0));
			sphere->updateInertiaTensor();
		}

		btTransform transform;
		sphere->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();

		moveVec[0] = -(moveVec[0] - vec[0]);
		moveVec[1] = -(moveVec[1] - vec[1]);
		moveVec[2] = -(moveVec[2] - vec[2]);

		m_pMover->Move(moveVec);
		col->updateHitboxPosition(moveVec);
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
	}
}
 

void PhysicsComponent::SetGravity(float x, float y, float z)
{
	s_pWorld->setGravity(btVector3(x, y,  z));
}

glm::vec3 PhysicsComponent::GetVelocity()
{
	if (m_pColission != nullptr)
	{
		btVector3 btVelocity = m_pColission->getBodyStructure()->body->getLinearVelocity();
		return glm::vec3(btVelocity[0], btVelocity[1], btVelocity[2]);
	}
	else
	{
		return glm::vec3(0, 0, 0);
	}
}