#include "DrawSystem.h"
#include "Paths.h"

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
	const float vertexPositions[] = {
		0.75f, 0.75f, 0.f,
		0.75f, -0.75f, 0.f,
		-0.75f, -0.75f, 0.f	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	(void)dt;

	//// START OpenGL test code
	// Draw
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Program);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	//// END OpenGL test Code
	
	// Iterate over all drawables and draw them
	
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

