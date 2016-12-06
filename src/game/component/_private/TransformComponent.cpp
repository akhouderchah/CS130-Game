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
	if(m_PosMatrix[3][0] == 1.f)
	{
		/*
		mat4 rotate = glm::toMat4(m_Orientation);
		
		m_PosMatrix = glm::translate(m_Position) * rotate * glm::scale(m_Scale);
		*/

		// Apply scale
		m_PosMatrix[0][0] = m_Scale[0];
		m_PosMatrix[0][1]=m_PosMatrix[0][2]=m_PosMatrix[0][3]=0;
		m_PosMatrix[1][0] = 0;
		m_PosMatrix[1][1] = m_Scale[1];
		m_PosMatrix[1][2]=m_PosMatrix[1][3]=0;
		m_PosMatrix[2][0]=m_PosMatrix[2][1]=0;
		m_PosMatrix[2][2] = m_Scale[2];
		m_PosMatrix[2][3]=0;
		m_PosMatrix[3][0]=m_PosMatrix[3][1]=m_PosMatrix[3][2] = 0;
		m_PosMatrix[3][3]=1;

		// Apply rotation
		// TODO - does the toMat4 function mix row-order and col-order matrices?
//		m_PosMatrix = (glm::toMat4(m_Orientation)) * m_PosMatrix;

		// Apply translation
//		mat4 translate(1.f); translate[0][3] = m_Position[0]; translate[1][3] = m_Position[1]; translate[2][3] = m_Position[2];
//		m_PosMatrix = translate * m_PosMatrix;
		
		m_PosMatrix[0][3] += m_Position[0];
		m_PosMatrix[1][3] += m_Position[1];
		m_PosMatrix[2][3] += m_Position[2];

	}

	return m_PosMatrix;
}

