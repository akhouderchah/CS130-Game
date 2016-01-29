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

	//// START OpenGL test code
	// Generate draw info
	const float vertexPositions[] = {
		0.75f, 0.75f, 0.f, 1.f,
		0.75f, -0.75f, 0.f, 1.f,
		-0.75f, -0.75f, 0.f, 1.f	};
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Draw
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);


	//// END OpenGL test Code
	
	// Iterate over all drawables and draw them
	
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

