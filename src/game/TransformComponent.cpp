#include "TransformComponent.h"

TransformComponent::TransformComponent(float x, float y) :
	m_Position(x, y)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Tick(deltaTime_t dt)
{
	(void)dt;
}

void TransformComponent::SetPosition(float x, float y)
{
	m_Position.x = x; m_Position.y = y;
}

void TransformComponent::SetPosition(const glm::vec2& position)
{
	m_Position = position;
}

const glm::vec2& TransformComponent::GetPosition()
{
	return m_Position;
}

