#include "MovableComponent.h"
#include <glm/gtx/euler_angles.hpp>

using namespace glm;

MovableComponent::MovableComponent(Entity entity) :
	IComponent(entity), m_pTransformComp(nullptr)
{
}

MovableComponent::~MovableComponent()
{
}

void MovableComponent::Refresh()
{
	m_pTransformComp = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

void MovableComponent::SetPosition(const vec3& position)
{
	assert(m_pTransformComp);
	m_pTransformComp->MarkDirty();
	m_pTransformComp->m_Position = position;
}

void MovableComponent::Move(const vec3& shift)
{
	assert(m_pTransformComp);
	m_pTransformComp->MarkDirty();
	m_pTransformComp->m_Position += shift;
}

void MovableComponent::SetOrientation(const vec3& radAngles)
{
	assert(m_pTransformComp);
	m_pTransformComp->MarkDirty();
	m_pTransformComp->m_Orientation = quat(radAngles);
}

void MovableComponent::Rotate(float rotationRads, const vec3& rotationAxis)
{
	assert(m_pTransformComp);
	m_pTransformComp->MarkDirty();	
	m_pTransformComp->m_Orientation = glm::angleAxis((rotationRads), rotationAxis) * m_pTransformComp->m_Orientation;
}

void MovableComponent::SetScale(const vec3& scale)
{
	assert(m_pTransformComp);
	m_pTransformComp->MarkDirty();	
	m_pTransformComp->m_Scale = scale;
}

void MovableComponent::Scale(const vec3& amount)
{
	assert(m_pTransformComp);
	m_pTransformComp->MarkDirty();	
	m_pTransformComp->m_Scale += amount;
}

