#include "PhysicsComponent.h"
#include "MovableComponent.h"

//@TODO, Check this website on how to detect collisions http://www.bulletphysics.org/mediawiki-1.5.8/index.php?title=Collision_Callbacks_and_Triggers

float PhysicsComponent::s_Gravity = -6.45f;
float PhysicsComponent::s_ImpulseSpeed = 2.85f;
float PhysicsComponent::s_TerminalSpeed = 4.75f;
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
	s_pCollisionConfig = new btDefaultCollisionConfiguration();
	s_pDispatcher = new btCollisionDispatcher(s_pCollisionConfig);
	s_pBroadphase = new btDbvtBroadphase();
	s_pSolver = new btSequentialImpulseConstraintSolver();
	s_pWorld = new btDiscreteDynamicsWorld(s_pDispatcher, s_pBroadphase, s_pSolver, s_pCollisionConfig);

	s_pWorld->setGravity(btVector3(0, -9.8, 0)); //Sets the gravity in the world
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
	//Check to see if there is a need to add new body to world
	if (m_pColission->getTotalBodies() > s_WorldObjectsID.size())
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
	

	std::vector<bulletObject *> bodies = m_pColission->getBodyVector();
	int zzz = 0;

	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		bodies[i]->hit = false;
	}

	s_pWorld->stepSimulation(1 / 5000.0); //FPS for the physics calculations in seconds

	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		if (bodies[i]->body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			renderPlane(bodies[i]);
			
		}
		else if (bodies[i]->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
		{
			renderBox(bodies[i]);
		}
		else if (bodies[i]->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			renderSphere(bodies[i]);
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
		bodies[i]->body->setLinearVelocity(btVector3(0.0, 5.0, 0.0));
	}
	return true;
}



void PhysicsComponent::renderPlane(bulletObject *obj)
{
	btRigidBody *plane = obj->body;

	if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
	{
		return;
	}
	else
	{
		btTransform transform;
		plane->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();

		//std::cout << vec[0] << " " << vec[1] << " " << vec[2] << std::endl;
	}
}

void PhysicsComponent::renderBox(bulletObject *obj)
{
	btRigidBody *box = obj->body;

	if (box->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE)
	{
		return;
	}
	else
	{
		btTransform transform;
		box->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();

		glm::vec3 moveVec = EntityManager::GetComponent<TransformComponent>(m_pMover->GetEntity())->GetPosition();

		moveVec[0] = -(moveVec[0] - vec[0]);
		moveVec[1] = -(moveVec[1] - vec[1]);
		moveVec[2] = -(moveVec[2] - vec[2]);

		//std::cout << vec[0] << " " << vec[1] << " " << vec[2] << std::endl;
		m_pMover->Move(moveVec);
		
		
	}
}


void PhysicsComponent::renderSphere(bulletObject *obj)
{
	btRigidBody *sphere = obj->body;

	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
	{
		return;
	}
	else
	{
		btTransform transform;
		sphere->getMotionState()->getWorldTransform(transform);

		btVector3 vec = transform.getOrigin();

		std::cout << vec[0] << " " << vec[1] << " " << vec[2] << std::endl;
	}
}


void PhysicsComponent::renderCylinder(bulletObject *obj)
{
	btRigidBody *cylinder = obj->body;

	if (cylinder->getCollisionShape()->getShapeType() != CYLINDER_SHAPE_PROXYTYPE)
	{
		return;
	}
	else
	{
		btTransform transform;
		cylinder->getMotionState()->getWorldTransform(transform);

		//Note that this actually puts cylinder on Y axis, so if you need to rotate, do it here.
		btVector3 vec = transform.getOrigin();
	}
}
