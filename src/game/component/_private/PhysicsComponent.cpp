#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_ImpulseSpeed = 2.85f;
float PhysicsComponent::s_ImpulseWaitTime = 0.0f;

btDynamicsWorld * PhysicsComponent::s_pWorld = nullptr;
btDispatcher * PhysicsComponent::s_pDispatcher = nullptr;
btCollisionConfiguration * PhysicsComponent::s_pCollisionConfig = nullptr;
btBroadphaseInterface * PhysicsComponent::s_pBroadphase = nullptr;
btConstraintSolver * PhysicsComponent::s_pSolver = nullptr;
std::vector<int> PhysicsComponent::s_WorldObjectsID;


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
	//static ConstVector<CollisionComponent*> tempCollision = EntityManager::GetAll<CollisionComponent>();
	/*
	for (int i = 0; i < tempCollision.size(); i++)
	{
		std::vector<bulletObject *> bodies = tempCollision[i]->getBodyVector();

		for (int i = 0; i < bodies.size(); i++)
		{
			s_pWorld->removeCollisionObject(bodies[i]->body);
			btMotionState *motionState = bodies[i]->body->getMotionState();
			btCollisionShape *shape = bodies[i]->body->getCollisionShape();

			delete bodies[i]->body;
			delete shape;
			delete motionState;
			delete bodies[i];
		}
	}
	*/

	//delete s_pDispatcher;
	//delete s_pCollisionConfig;
	//delete s_pSolver;
	//delete s_pBroadphase;
	//delete s_pWorld;

	int i = 0;

}

void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pColission = EntityManager::GetComponent<CollisionComponent>(m_Entity);
}

void PhysicsComponent::Tick(deltaTime_t dt)
{

	//Check to see if there is a need to add new body to the world
	if (m_pColission->getTotalBodies() > s_WorldObjectsID.size())
	{
		updateWorldObjects();
	}
	
	std::vector<bulletObject *> bodies = m_pColission->getBodyVector();

	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		bodies[i]->hit = false;
	}

	s_pWorld->stepSimulation(1.0 / 5000.0); //FPS for the physics calculations in seconds

	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		if (bodies[i]->body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			updatePlane(bodies[i]);		
		}
		else if (bodies[i]->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
		{
			updateBox(bodies[i]);
		}
		else if (bodies[i]->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			updateSphere(bodies[i], m_pColission);
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

	std::vector<bulletObject *> bodies = m_pColission->getBodyVector();
	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		bodies[i]->body->activate();
		bodies[i]->body->setLinearVelocity(btVector3(0.0, 5.0, 0.0));
	}
	return true;
}

bool PhysicsComponent::ImpulseRight()
{
	if (m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;

	std::vector<bulletObject *> bodies = m_pColission->getBodyVector();
	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		bodies[i]->body->activate();
		bodies[i]->body->setLinearVelocity(btVector3(1.0, 0.0, 0.0));
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

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();

		moveVec[0] = -(moveVec[0] - vec[0]);
		moveVec[1] = -(moveVec[1] - vec[1]);
		moveVec[2] = -(moveVec[2] - vec[2]);

		btMatrix3x3 mat = transform.getBasis();

		m_pMover->Move(moveVec);
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

		/*
		btMatrix3x3 mat = transform.getBasis();

		for (int i = 0; i < 3; i++)
		{
			std::cout << mat[i][0] << " " << mat[i][1] << " " << mat[i][2] << std::endl;
		}
		std::cout << std::endl << std::endl;
		//std::cout << "Sphere" << obj->id << " " << vec[0] << " " << vec[1] << " " << vec[2] << std::endl;
		*/
		m_pMover->Move(moveVec);
		col->toggleHitboxView(moveVec);
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
		btVector3 btVelocity = m_pColission->getBodyVector()[0]->body->getLinearVelocity();
		return glm::vec3(btVelocity[0], btVelocity[1], btVelocity[2]);
	}
	else
	{
		return glm::vec3(0, 0, 0);
	}
}


void PhysicsComponent::updateWorldObjects()
{
	ConstVector<CollisionComponent*> tempCollision = EntityManager::GetAll<CollisionComponent>();
	for (unsigned int i = 1; i < tempCollision.size(); i++)
	{
		std::vector<bulletObject *> bodies = tempCollision[i]->getBodyVector();

		for (unsigned int i = 0; i < bodies.size(); i++)
		{
			//We are looking for bodies that were not added to the world
			if (std::find(s_WorldObjectsID.begin(), s_WorldObjectsID.end(), bodies[i]->id) == s_WorldObjectsID.end())
			{
				s_pWorld->addRigidBody(bodies[i]->body);
				s_WorldObjectsID.push_back(bodies[i]->id);
			}
		}
	}
}