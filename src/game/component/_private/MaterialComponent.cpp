#include "MaterialComponent.h"
#include "DrawComponent.h"

MaterialComponent::MaterialComponent(Entity entity) :
	IComponent(entity), m_pDrawComp(nullptr), m_T(0.f)
{
}

MaterialComponent::~MaterialComponent()
{
}

void MaterialComponent::FadeIn(deltaTime_t fadeTime)
{
	DEBUG_ASSERT(fadeTime > 0.f);
	m_T = fadeTime;
}

void MaterialComponent::FadeOut(deltaTime_t fadeTime)
{
	DEBUG_ASSERT(fadeTime > 0.f);
	m_T = -1.f * fadeTime;
}

void MaterialComponent::Refresh()
{
	m_pDrawComp = EntityManager::GetComponent<DrawComponent>(m_Entity);
}

void MaterialComponent::Tick(deltaTime_t dt)
{
	if(m_T != 0.f)
	{
	    m_T *= !(m_pDrawComp->IncrementOpacity(dt/m_T));
	}
}
