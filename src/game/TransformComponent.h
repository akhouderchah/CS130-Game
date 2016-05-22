#pragma once

#include "../system/IComponent.h"
#include "../base/Base.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class TransformComponent : public IComponent
{
public:
	TransformComponent(const glm::vec3& position = glm::vec3(0, 0, 0),
					   const glm::vec3& scale = glm::vec3(1, 1, 1));
	~TransformComponent();

	void Tick(deltaTime_t dt);
	void Destroy(){}
	const glm::mat4& GetWorldMatrix() const;
	const glm::mat4& GetCameraMatrix() const;

	/// Translation functions
	void SetPosition(const glm::vec3& position);
	void Move(const glm::vec3& shift);
	inline const glm::vec3& GetPosition() const{ return m_Position; }

	/// Orientation functions
	void SetOrientation(const glm::vec3& radAngles);
	void Rotate(float rotationRads, const glm::vec3& rotationAxis);
	inline const glm::quat& GetOrientation() const{ return m_Orientation; }

	/// Scale functions
	void SetScale(const glm::vec3& scale);
	void Scale(const glm::vec3& amount);
	inline const glm::vec3& GetScale() const{ return m_Scale; }
private:
	inline void MarkDirty(){ m_PosMatrix[3][0] = 1.f; }

private:
	//TransformComponent(const TransformComponent& that);
	//TransformComponent& operator=(const TransformComponent& that);

	// Actual transform data goes here //
	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::vec3 m_Scale;
	mutable glm::mat4 m_PosMatrix; // We use [3][0] as a dirty flag
};
