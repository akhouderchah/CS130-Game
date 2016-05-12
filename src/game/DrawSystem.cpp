#include "DrawSystem.h"
#include "Paths.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
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

	// Generate draw info
	const float angle = glm::pi<float>()/6;
	const float cosx = cosf(angle);
	const float sinx = sinf(angle);
	const glm::vec3 vertices[] = {
		glm::vec3(-cosx, -sinx/2 - 0.5f, -sinx),
		glm::vec3(0.f, -sinx/2 - 0.5f, cosx),
		glm::vec3(cosx, -sinx/2 - 0.5f, -sinx),
		glm::vec3(0.f, 0.5f+glm::root_three<float>()/4, 0.f) };
	
	unsigned int indices [] = { 0, 3, 1,
								1, 3, 2,
								2, 3, 0,
								0, 1, 2 };
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

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
	
	//// START OpenGL test code
	// Draw
	static GLuint worldLoc = glGetUniformLocation(m_Program, "gWorld");
	if(worldLoc == 0xFFFFFFFF){ return; }
	
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
	glUniformMatrix4fv(worldLoc, 1, GL_TRUE, &World[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	//// END OpenGL test Code
	
	// Iterate over all drawables and draw them
	
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

