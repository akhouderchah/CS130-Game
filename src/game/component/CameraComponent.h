#pragma once

#include "IComponent.h"

class TransformComponent;

/**
 * @brief Component that gives an entity the ability to act as a camera
 *
 * The render system uses information from this component in order to
 * generate the final MVP matrix. Having this as a component makes it
 * easy to switch between cameras as needed at runtime.
 */
class CameraComponent : public IComponent
{
public:
	CameraComponent(Entity entity);
	~CameraComponent();

	enum EProjectionType
	{
		EPT_PERSPECTIVE,
		EPT_ORTHOGRAPHIC,
	};
	EProjectionType GetProjectionType() const{ return m_ProjectionType; }
	void SetProjectionType(EProjectionType projectionType);

	float GetFOV() const{ return m_FOV; }
	void SetFOV(float FOV){ m_FOV = FOV; }

	float GetNear() const{ return m_Near; }
	void SetNear(float nearDistance){ m_Near = nearDistance; }

	float GetFar() const{ return m_Far; }
	void SetFar(float farDistance){ m_Far = farDistance; }

	const glm::mat4& GetCameraMatrix() const;

	void Refresh();

	/**
	 * @brief Inform all camera components of the render screen size
	 *
	 * Should be called whenever the screen size is changed
	 *
	 * @note Placed here instead of in the DrawSystem because this information
	 * is needed to create the camera matrix. Since components should not
	 * know about systems, it does not make sense to place this information
	 * in the DrawSystem unless the DrawSystem takes care of calculating the
	 * camera matrix (possible, but inconvenient).
	 */
	static void SetWindowSize(int32_t width, int32_t height);

private:
	TransformComponent *m_pTransformComp;
	EProjectionType m_ProjectionType;
	mutable glm::mat4 m_CameraMatrix;
	float m_FOV;
	float m_Near;
	float m_Far;

	static int32_t s_ScreenWidth;
	static int32_t s_ScreenHeight;
};
