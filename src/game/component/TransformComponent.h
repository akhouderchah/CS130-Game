#pragma once

#include "Base.h"
#include "IComponent.h"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class MovableComponent;

/**
 * @brief Component that allows an entity to exist physically in the game world
 *
 * Gives an entity a position, scale, and orientation. While certain entities
 * may not need this, anything that will physically exist in the game world will
 * need to have this component.
 */
class TransformComponent : public IComponent
{
public:
	TransformComponent(Entity entity);
	~TransformComponent();

	bool Init(const glm::vec3& position = glm::vec3(0,0,0),
			  const glm::vec3& scale = glm::vec3(1,1,1));
	void Refresh(){}
	const glm::mat4& GetWorldMatrix() const;

	inline void MarkDirty(){ m_PosMatrix[0][3] = 1.f; }
	inline bool IsDirty(){ return m_PosMatrix[0][3] == 1.f; }

	inline const glm::vec3& GetPosition() const{ return m_Position; }
	inline const glm::quat& GetOrientation() const{ return m_Orientation; }
	inline const glm::vec3& GetScale() const{ return m_Scale; }

private:
	//TransformComponent(const TransformComponent& that);
	//TransformComponent& operator=(const TransformComponent& that);

	friend class MovableComponent;

	// Actual transform data goes here //
	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::vec3 m_Scale;
	mutable glm::mat4 m_PosMatrix; // We use [3][0] as a dirty flag
};
