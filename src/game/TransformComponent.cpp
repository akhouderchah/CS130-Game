#include "TransformComponent.h"

using glm::vec3; using glm::quat;
using glm::mat4;

TransformComponent::TransformComponent(const vec3& position,
									   const vec3& scale) :
	m_Position(position), m_Scale(scale)
{
	MarkDirty();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Tick(deltaTime_t dt)
{
	(void)dt;
}

const mat4& TransformComponent::GetWorldMatrix() const
{
	if(m_PosMatrix[3][0] == 1.f) // We test for equality only because we explicitly set to 1.f
	{
		// Reset matrix
		m_PosMatrix = mat4();
		
		// Apply scale
		m_PosMatrix[0][0] = m_Scale[0];
		m_PosMatrix[1][1] = m_Scale[1];
		m_PosMatrix[2][2] = m_Scale[2];

		// Apply rotation
		// TODO - does the toMat4 function mix row-order and col-order matrices?
		m_PosMatrix = glm::toMat4(m_Orientation) * m_PosMatrix;

		// Apply translation
		m_PosMatrix[0][3] += m_Position[0];
		m_PosMatrix[1][3] += m_Position[1];
		m_PosMatrix[2][3] += m_Position[2];
	}
	
	return m_PosMatrix;
}

void TransformComponent::SetPosition(const vec3& position)
{
	MarkDirty();
	m_Position = position;
}

void TransformComponent::Move(const vec3& shift)
{
	MarkDirty();
	m_Position += shift;
}

void TransformComponent::SetOrientation(const vec3& radAngles)
{
	m_Orientation = quat(radAngles);
}

void TransformComponent::Rotate(float rotationRads, const vec3& rotationAxis)
{
	m_Orientation = glm::angleAxis(glm::degrees(rotationRads), rotationAxis);
}

void TransformComponent::SetScale(const vec3& scale)
{
	MarkDirty();
	m_Scale = scale;
}

void TransformComponent::Scale(const vec3& amount)
{
	MarkDirty();
	m_Scale += amount;
}

