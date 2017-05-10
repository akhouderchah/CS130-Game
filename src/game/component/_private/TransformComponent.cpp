#include "TransformComponent.h"

#include <glm/gtx/transform.hpp>

using glm::vec3; using glm::quat;
using glm::mat4;

TransformComponent::TransformComponent(Entity entity) :
	IComponent(entity)
{
	MarkDirty();
}

TransformComponent::~TransformComponent()
{
}

bool TransformComponent::Init(const vec3& position, const vec3& scale)
{
	m_Position = position;
	m_Scale = scale;
	return true;
}

const mat4& TransformComponent::GetWorldMatrix() const
{
	if(m_PosMatrix[0][3] == 1.f)
	{
		m_PosMatrix = glm::translate(m_Position) *
			glm::mat4_cast(m_Orientation) *
			glm::scale(m_Scale);
	}

	return m_PosMatrix;
}

