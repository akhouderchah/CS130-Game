#include "DrawSystem.h"
#include "Paths.h"
#include "LogSystem.h"

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
	ShaderProgram program(2);
	program.AddShader(GL_VERTEX_SHADER, TEST_VERTEX_PATH);
	program.AddShader(GL_FRAGMENT_SHADER, TEST_FRAG_PATH);
	m_Program = program.Compile();
	if(m_Program == GL_NONE)
		return false;

	m_WorldLoc = glGetUniformLocation(m_Program, "gWorld");
	if(m_WorldLoc == 0xFFFFFFFF){ return false; }

	return true;
}

void DrawSystem::Shutdown()
{
	glDeleteProgram(m_Program);
}

void DrawSystem::Tick(deltaTime_t dt)
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(m_Program);

	// Set scale
	static float scale = 0.f;
	scale += dt * 1.f;

	// Set world matrix
	glm::mat4 World;
	World[0][0] = cosf(scale); World[0][1] = 0.f; World[0][2] = -sinf(scale); World[0][3] = 0.f;
	World[1][0] = 0.f; World[1][1] = 1.f; World[1][2] = 0.f; World[1][3] = 0.f;
	World[2][0] = sinf(scale); World[2][1] = 0.f; World[2][2] = cosf(scale); World[2][3] = 0.f;
	World[3][0] = 0.f; World[3][1] = 0.f; World[3][2] = 0.f; World[3][3] = 1.f;
	glUniformMatrix4fv(m_WorldLoc, 1, GL_TRUE, &World[0][0]);

	// Iterate over all drawables and draw them
	for(size_t i = 0; i < m_DrawComponents.size(); ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_DrawComponents[i].first.m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_DrawComponents[i].first.m_IBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
	}
	glUseProgram(0);
	
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

