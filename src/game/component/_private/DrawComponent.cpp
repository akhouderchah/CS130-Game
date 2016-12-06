#include "DrawComponent.h"
#include "TransformComponent.h"

#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

using glm::vec3; using glm::vec2;

bool initialized = false;

GLuint VBO, IBO, VertexArrayID, Tex;

DrawComponent::DrawComponent(Entity entity) :
	IComponent(entity), m_pTransformComp(nullptr)
{
    static const DrawComponent::Vertex vertices[] = {
	    { vec3(-1.0f, -1.0f, +0.5f), vec2(0.0f, 0.0f) },
	    { vec3(+0.0f, -1.0f, -1.2f), vec2(1.0f, 0.0f) },
	    { vec3(+1.0f, -1.0f, +0.5f), vec2(0.0f, 1.0f) },
	    { vec3(+0.0f, +1.0f, +0.0f), vec2(1.0f, 1.0f) },
	};
	static const unsigned int indices [] = { 0, 3, 1,
									         1, 3, 2,
									         2, 3, 0,
											 0, 1, 2 };

	if(!initialized && entity != nullEntity)
	{
		int comp, h, w;
		unsigned char *pImage;

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

		// Load texture
		pImage = stbi_load("../../assets/textures/Background.tga", &w, &h,
							 &comp, STBI_rgb);

		glGenTextures(1, &m_Tex);
		glBindTexture(GL_TEXTURE_2D, m_Tex);
		Tex = m_Tex;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if(comp == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
		else if(comp == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(pImage);

		initialized = true;
	}

	m_VBO = VBO;
	m_IBO = IBO;
	m_Tex = Tex;
}

DrawComponent::~DrawComponent()
{
}

void DrawComponent::Refresh()
{
	m_pTransformComp = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

