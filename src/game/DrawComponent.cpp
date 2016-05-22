#include "DrawComponent.h"

bool initialized = false;

const glm::vec3 vertices[] = {
		glm::vec3(-1, -0.5f, 0),
		glm::vec3(0.f, -0.5f, 1),
		glm::vec3(1, - 0.5f, 0),
		glm::vec3(0.f, 0.5f, 0.f) };
	
const unsigned int indices [] = { 0, 3, 1,
								  1, 3, 2,
								  2, 3, 0,
								  0, 1, 2 };

GLuint VBO, IBO, VertexArrayID;

DrawComponent::DrawComponent()
{
	if(!initialized)
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		initialized = true;
	}

	m_VBO = VBO;
	m_IBO = IBO;
}

DrawComponent::~DrawComponent()
{
}

void DrawComponent::Destroy()
{
}

