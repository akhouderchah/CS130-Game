#include "MaterialComponent.h"
#include "DrawComponent.h"

MaterialComponent::MaterialComponent(Entity entity) :
	IComponent(entity), m_T(0.f), m_Opacity(1.f),
	m_Time(0.f), m_ScrollRate(0.f)
{
}

MaterialComponent::~MaterialComponent()
{
}

bool MaterialComponent::SetOpacity(float opacity)
{
	m_Opacity = opacity;
	return IncrementOpacity(0.f);
}

bool MaterialComponent::IncrementOpacity(float delta)
{
	m_Opacity += delta;

	if(m_Opacity < 0.f)
	{
		m_Opacity = 0.f;
		return true;
	}
	else if(m_Opacity > 1.f)
	{
		m_Opacity = 1.f;
		return true;
	}

	return false;
}

void MaterialComponent::FadeIn(deltaTime_t fadeTime)
{
	DEBUG_ASSERT(fadeTime > 0.f);
	RELEASE_ASSERT(GetEntity() != nullEntity);
	m_T = fadeTime;
}

void MaterialComponent::FadeOut(deltaTime_t fadeTime)
{
	DEBUG_ASSERT(fadeTime > 0.f);
	RELEASE_ASSERT(GetEntity() != nullEntity);
	m_T = -1.f * fadeTime;
}

void MaterialComponent::Refresh()
{
}

void MaterialComponent::Tick(deltaTime_t dt)
{
	if(m_T != 0.f)
	{
	    m_T *= !(IncrementOpacity(dt/m_T));
	}

	if(m_ScrollRate != 0.f)
	{
		m_Time += m_ScrollRate * dt;
	}
}
