#include "CameraComponent.h"
#include "TransformComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

int32_t CameraComponent::s_ScreenWidth = 0;
int32_t CameraComponent::s_ScreenHeight = 0;

#define DEFAULT_FOV 45.f
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 1000.f

CameraComponent::CameraComponent(Entity entity) :
	IComponent(entity), m_pTransformComp(nullptr),
	m_ProjectionType(EPT_PERSPECTIVE), m_FOV(DEFAULT_FOV),
	m_Near(DEFAULT_NEAR), m_Far(DEFAULT_FAR)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetProjectionType(EProjectionType projectionType)
{
	m_ProjectionType = projectionType;
}

void CameraComponent::Refresh()
{
	m_pTransformComp = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

void CameraComponent::SetWindowSize(int32_t width, int32_t height)
{
	s_ScreenWidth = width;
	s_ScreenHeight = height;
}

const glm::mat4& CameraComponent::GetCameraMatrix() const
{
	// NOTE: This is contingent on the DrawSystem getting the camera
	// matrix before getting the entity's world matrix. This shouldn't
	// be an issue, because it makes sense for the DrawSystem to get the
	// matrices in that order. But it is something to keep in mind when
	// an entity has both DrawComponent and CameraComponent.
	if(m_pTransformComp->IsDirty())
	{
		// Generate view matrix
		const mat3 orientationMatrix = toMat3(m_pTransformComp->GetOrientation());

		vec3 facingDir(0, 0, -1);
		facingDir = normalize(orientationMatrix * facingDir);

		vec3 upDir(0, 1, 0);
		upDir = normalize(orientationMatrix * upDir);

		const vec3 &cameraPos = m_pTransformComp->GetPosition();
		m_CameraMatrix = lookAt(cameraPos,
								cameraPos + facingDir,
								upDir);

		// Add projection matrix on the left side
		switch(m_ProjectionType)
		{
		case EPT_PERSPECTIVE:
			m_CameraMatrix = perspectiveFov(m_FOV, (float)s_ScreenWidth, (float)s_ScreenHeight, m_Near, m_Far) * m_CameraMatrix;
			break;
		case EPT_ORTHOGRAPHIC:
			m_CameraMatrix = ortho(0.f, (float)s_ScreenWidth, (float)s_ScreenHeight, 0.f, m_Near, m_Far) * m_CameraMatrix;
			break;
		default:
			DEBUG_LOG("Camera: Invalid perspective type - " << m_ProjectionType << "\n");
		}
	}

	return m_CameraMatrix;
}
