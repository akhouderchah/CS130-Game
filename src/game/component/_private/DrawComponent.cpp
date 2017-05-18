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

void DrawComponent::SetTexture(std::string texture, TextureType type)
{
	m_Tex = ResourceManager::LoadTexture(texture, type);
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

