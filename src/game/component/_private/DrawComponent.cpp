#include "DrawComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

using glm::vec3; using glm::vec2;

DrawComponent::DrawComponent(Entity entity) :
	IComponent(entity), m_pTransformComp(nullptr),
	m_VBO(0), m_IBO(0), m_Tex(0)
{
	/*
    static const DrawComponent::Vertex vertices[] = {
	    { vec3(-1.0f, -1.0f, +0.5f), vec2(0.0f, 0.0f) },
	    { vec3(+0.0f, -1.0f, -1.2f), vec2(1.0f, 0.0f) },
	    { vec3(+1.0f, -1.0f, +0.5f), vec2(0.0f, 1.0f) },
	    { vec3(+0.0f, +1.0f, +0.0f), vec2(1.0f, 1.0f) },
	};
	static const unsigned int indices [] = { 0, 3, 1,
									         1, 3, 2,
									         2, 3, 0,
											 0, 1, 2 };
	*/

	m_timer = std::chrono::system_clock::now();
	currentTextureIndex = 0;
}

DrawComponent::~DrawComponent()
{
}

void DrawComponent::SetGeometry(ShapeType shape)
{
	auto model = ResourceManager::LoadShape(shape);
	m_VBO = model.first;
	m_IBO = model.second;
}

void DrawComponent::SetTexture(std::string texture, TextureType type, int animationTimer)
{
	m_TexVector.emplace_back(ResourceManager::LoadTexture(texture, type));
	m_Tex = m_TexVector[0];

	m_AnimationTimer = animationTimer;
}

float DrawComponent::GetOpacity() const
{
	return m_pMaterialComp->m_Opacity;
}

float DrawComponent::GetTime() const
{
	return m_pMaterialComp->m_Time;
}

void DrawComponent::Refresh()
{
	m_pTransformComp = EntityManager::GetComponent<TransformComponent>(m_Entity);
	m_pMaterialComp = EntityManager::GetComponent<MaterialComponent>(m_Entity);
}

void DrawComponent::changeTexture()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


	auto duration2 = m_timer.time_since_epoch();
	auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2).count();

	
	if (millis >= millis2 + m_AnimationTimer)
	{
		std::cout << "Changing textures" << std::endl;
		
		if (currentTextureIndex >= m_TexVector.size() - 1)
		{
			currentTextureIndex = 0;
			m_Tex = m_TexVector[currentTextureIndex];
		}
		else
		{
			currentTextureIndex += 1;
			m_Tex = m_TexVector[currentTextureIndex];
		}
		
		m_timer = std::chrono::system_clock::now();
	}
}