#include "MovableComponent.h"
#include "CollisionComponent.h"
#include "DrawComponent.h"

#include "EntityManager.h"
#include "MaterialComponent.h"

unsigned int CollisionComponent::s_LastBodyID = 0;
unsigned int CollisionComponent::s_TotalBodies = 0;


CollisionComponent::CollisionComponent(Entity entity) :
	IComponent(entity),  m_pMover(nullptr), m_pDraw(nullptr),
	m_pTransform(nullptr)
{
	isCollisionViewEnabled = true;

	m_OldPosition = EntityManager::GetComponent<TransformComponent>(m_Entity)->GetPosition();
	m_OldScale = EntityManager::GetComponent<TransformComponent>(m_Entity)->GetScale();
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






void CollisionComponent::EnableHitboxView()
{
	/*
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(m_AdditionToPosition + m_pTransform->GetPosition()), glm::vec3(m_AdditionToScale + m_pTransform->GetScale()));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);
	//toggleHitboxView();
	*/
}


void CollisionComponent::toggleHitboxView(glm::vec3 vec)
{
	/*
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(entity);
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
	
	move->Move(vec);
	*/

	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(entity);
	move->Move(vec);
}



void CollisionComponent::addPlane(float additionToX, float additionToY, float additionToZ)
{
	btTransform transform; 
	transform.setIdentity();

	//transform.setOrigin(btVector3(m_pTransform->GetPosition()[0] + additionToX, m_pTransform->GetPosition()[1] + additionToY, m_pTransform->GetPosition()[2] + additionToZ));
	transform.setOrigin(btVector3(additionToX, additionToY, additionToZ));

	btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	btMotionState *motion = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody *body = new btRigidBody(info);

	m_pBodies.push_back(new bulletObject(body,HitboxShapeType::PLANE, s_LastBodyID, false));
	s_LastBodyID += 1;
	s_TotalBodies += 1;


	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(additionToX, additionToY, additionToZ), glm::vec3(0.1, 0.1, 0.1));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);

}


void CollisionComponent::addBox(float mass, float additionTowidth, float additionToheight, float additionTodepth, float additionToX, float additionToY, float additionToZ, bool enableRotation)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(m_pTransform->GetPosition()[0] + additionToX, m_pTransform->GetPosition()[1] + additionToY, m_pTransform->GetPosition()[2] + additionToZ));

	//btBoxShape *box = new btBoxShape(btVector3( (m_pTransform->GetScale()[0] + additionTowidth) / 2.0, (m_pTransform->GetScale()[1] + additionToheight) / 2.0, (m_pTransform->GetScale()[2] + additionTodepth) / 2.0));
	btBoxShape *box = new btBoxShape(btVector3((additionTowidth), (additionToheight), (additionTodepth)));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		box->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);
	

	m_pBodies.push_back(new bulletObject(body, HitboxShapeType::BOX, s_LastBodyID, enableRotation));
	s_LastBodyID += 1;
	s_TotalBodies += 1;


}

void CollisionComponent::addSphere(float mass, float radius, float additionToX, float additionToY, float additionToZ, bool enableRotation)
{
	btTransform transform;
	transform.setIdentity();

	//transform.setOrigin(btVector3(m_pTransform->GetPosition()[0] + additionToX, m_pTransform->GetPosition()[1] + additionToY, m_pTransform->GetPosition()[2] + additionToZ));
	transform.setOrigin(btVector3(additionToX, additionToY, additionToZ));


	btSphereShape *sphere = new btSphereShape(radius);

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		sphere->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);//note that we passed mass to not make it static
	btRigidBody *body = new btRigidBody(info);

	m_pBodies.push_back(new bulletObject(body, HitboxShapeType::SPHERE, s_LastBodyID, enableRotation));
	s_LastBodyID += 1;
	s_TotalBodies += 1;


	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(additionToX, additionToY, additionToZ), glm::vec3(radius, radius, radius));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "circle.tga", TextureType::RGBA);
}


void CollisionComponent::addCylinder(float diameter, float height, float x, float y, float z, float mass, bool enableRotation)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));

	btCylinderShape *cylinder = new btCylinderShape(btVector3(diameter / 2.0, height / 2.0, diameter / 2.0));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		cylinder->calculateLocalInertia(mass, inertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
	btRigidBody *body = new btRigidBody(info);

	m_pBodies.push_back(new bulletObject(body, HitboxShapeType::CYLINDER, s_LastBodyID, enableRotation));
	s_LastBodyID += 1;
	s_TotalBodies += 1;
}






























