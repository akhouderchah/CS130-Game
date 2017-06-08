#include "DrawSystem.h"
#include "EntityManager.h"
#include "Paths.h"
#include "PhysicsComponent.h"
#include "LogSystem.h"
#include "CameraComponent.h"

#include <ft2build.h>
#include "FreeType.h"
#include <map>

// GLM
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint vertexArrayID, VBO, IBO;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

DrawSystem::DrawSystem() :
	m_pDrawComponents(EntityManager::GetAll<DrawComponent>()),
	m_pMaterialComponents(EntityManager::GetAll<MaterialComponent>()),
	m_pCurrentCamera(nullptr)
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

	m_TextureLoc = glGetUniformLocation(m_Program, "gSampler");
	if(m_TextureLoc == 0xFFFFFFFF){ return false; }

	m_AlphaLoc = glGetUniformLocation(m_Program, "gAlpha");
	if(m_AlphaLoc == 0xFFFFFFFF){ return false; }

	m_TimeLoc = glGetUniformLocation(m_Program, "gTime");
	if(m_TimeLoc == 0xFFFFFFFF){ return false; }

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize FreeType and load fonts. 
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, STANDARD_FONT_PATH.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as.
	FT_Set_Pixel_Sizes(face, 0, 100);

	// Disable byte-alignment restriction since we are using a single byte
	// per pixel. 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++) 
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		// Create a texture where each byte corresponds 
		// to the texture color's red component.
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	return true;
}

void DrawSystem::Shutdown()
{
	glDeleteProgram(m_Program);
}

void DrawSystem::Tick(deltaTime_t dt)
{
	//Getting the current camera
	m_pCurrentCamera = CameraComponent::GetCurrentCamera();

	DEBUG_ASSERT(m_pCurrentCamera);

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Program);

	// Set scale
	static float scale = 0.f;
	scale -= dt * 1.f;

	//static MovableComponent* pMove = EntityManager::GetComponent<MovableComponent>(m_pDrawComponents[1].second);
	//pMove->SetOrientation(glm::vec3(4*sin(scale), 0, 0));

	// Update all materials
	for(size_t i = 1; i < m_pMaterialComponents.size(); ++i)
	{
		m_pMaterialComponents[i]->Tick(dt);
	}

	static glm::mat4 MVP;
	const glm::mat4& cameraMat = m_pCurrentCamera->GetCameraMatrix();

	ResourceManager::LoadShape(ShapeType::PLANE);

	// Iterate over all drawables and draw them
	for(size_t i = 1; i < m_pDrawComponents.size(); ++i)
	{
		DEBUG_ASSERT(m_pDrawComponents[i]->m_pTransformComp);

		// Update material globals in shaders
		glUniform1f(m_AlphaLoc, m_pDrawComponents[i]->GetOpacity());
		glUniform1f(m_TimeLoc, m_pDrawComponents[i]->GetTime());

		// Create final MVP matrix
		//
		// This could be done in the vertex shader, but would result in
		// duplicating this computation for every vertex in a model
		MVP = cameraMat * m_pDrawComponents[i]->m_pTransformComp->GetWorldMatrix();
		glUniformMatrix4fv(m_WorldLoc, 1, GL_FALSE, &MVP[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, m_pDrawComponents[i]->m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pDrawComponents[i]->m_IBO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pDrawComponents[i]->m_Tex);
		glUniform1i(m_TextureLoc, 0);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}


	glUniform3f(glGetUniformLocation(m_Program, "textColor"), 0.9f, 0.1f, 0.3f);
	glUniform3f(m_TextureLoc, 0.9, 0.1f, 0.3f);

	std::string::const_iterator c;
	std::string text = "Tetrad";
	GLfloat x = 0.f, y = 0.f;

	scale = 0.2f;

	glm::mat4 projection = cameraMat * glm::translate(glm::vec3(-1.5, 2, 0));
	glm::mat4 scalingMatrix = glm::scale(glm::vec3(0.08f, 0.08f, 0.01f));
	projection *= scalingMatrix;
	glUniformMatrix4fv(m_WorldLoc, 1, GL_FALSE, glm::value_ptr(projection));

	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;



		glUniform1f(m_AlphaLoc, 0.5f);
		glUniform1f(m_TimeLoc, 0.f);


		DrawComponent::Vertex vertices[] = {
			{ glm::vec3(xpos, ypos + h, 0.f), glm::vec2(0.0, 0.0) },
			{ glm::vec3(xpos, ypos, 0.f), glm::vec2(0.0, 1.0) },
			{ glm::vec3(xpos + w, ypos, 0.f), glm::vec2(1.0, 1.0) },
			{ glm::vec3(xpos + w, ypos + h, 0.f), glm::vec2(1.0, 0.0) }
		};

		unsigned int indices[] = { 2, 1, 0,
			0, 3, 2 };


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE,
			sizeof(DrawComponent::Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE,
			sizeof(DrawComponent::Vertex),
			(const GLvoid*)sizeof(glm::vec3));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		x += (ch.Advance >> 6) * scale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);


	glUseProgram(0);
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

