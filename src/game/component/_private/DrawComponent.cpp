#include "DrawComponent.h"
#include "TransformComponent.h"

#include "SOIL.h"

using glm::vec3; using glm::vec2;

bool initialized = false;

GLuint VBO, IBO, VertexArrayID;

DrawComponent::DrawComponent(Entity entity) :
	IComponent(entity), m_pTransformComp(nullptr)
{
    static const DrawComponent::Vertex vertices[] = {
	    { vec3(-1.0f, -1.0f, +0.5f), vec2(0.0f, 0.0f) },
	    { vec3(+0.0f, -1.0f, -1.2f), vec2(0.5f, 0.0f) },
	    { vec3(+1.0f, -1.0f, +0.5f), vec2(1.0f, 0.0f) },
	    { vec3(+0.0f, +1.0f, +0.0f), vec2(0.5f, 1.0f) },
	};
	static const unsigned int indices [] = { 0, 3, 1,
									         1, 3, 2,
									         2, 3, 0,
											 0, 1, 2 };

	if(!initialized && entity != nullEntity)
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(DrawComponent::Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(DrawComponent::Vertex), (const GLvoid*)sizeof(glm::vec3));



		m_Tex = SOIL_load_OGL_texture("../../assets/textures/Background.tga",
										   SOIL_LOAD_AUTO,
										   SOIL_CREATE_NEW_ID,
										   SOIL_FLAG_MIPMAPS
			);

        if(m_Tex == 0){ ERROR("Failed to get texture!\n", EEB_CONTINUE); }

		initialized = true;
	}

	m_VBO = VBO;
	m_IBO = IBO;
}

DrawComponent::~DrawComponent()
{
}

void DrawComponent::Refresh()
{
	m_pTransformComp = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

