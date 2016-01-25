#include "DrawSystem.h"
#include <iostream>

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
	// Clear screen
	(void)dt;

	// Iterate over all drawables and draw them
	
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

