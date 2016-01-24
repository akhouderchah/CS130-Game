#include "DrawSystem.h"
#include <iostream>

using namespace sf;

DrawSystem::DrawSystem() :
	m_DrawComponents(ComponentManager<DrawComponent>::GetAll()),
	m_TransformComponents(ComponentManager<TransformComponent>::GetAll())
{
}

DrawSystem::~DrawSystem()
{
}

bool DrawSystem::Initialize()
{
	return true;
}

void DrawSystem::Shutdown()
{
}

void DrawSystem::Tick(deltaTime_t dt)
{
	m_pWindow->clear();
	(void)dt;
	// Iterate over all drawables and draw them
	for(size_t i = 0; i < m_DrawComponents.size(); ++i)
	{
		auto drawables = m_DrawComponents[i].first.GetDrawables();
		for(size_t j = 0; j < drawables.size(); ++j)
		{
			m_pWindow->draw(*drawables[j]);
		}
	}
	m_pWindow->display();
}

