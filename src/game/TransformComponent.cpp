#include "TransformComponent.h"

using namespace sf;

TransformComponent::TransformComponent(float x, float y) :
	m_Position(x, y)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Tick(deltaTime_t dt)
{
}

void TransformComponent::SetPosition(float x, float y)
{
	m_Position.x = x; m_Position.y = y;
}

void TransformComponent::SetPosition(Vector2f position)
{
	m_Position = position;
}

const Vector2f& TransformComponent::GetPosition()
{
	return m_Position;
}

