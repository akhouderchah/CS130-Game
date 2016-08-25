#include "DrawSystem.h"
#include "Paths.h"
#include "LogSystem.h"

#include "EntityManager.h"
#include "PhysicsComponent.h"

DrawSystem::DrawSystem() :
	m_pDrawComponents(EntityManager::GetAll<DrawComponent>())
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
	
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

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
	
	//static MovableComponent* pMove = EntityManager::GetComponent<MovableComponent>(m_pDrawComponents[1].second);
	//pMove->SetOrientation(glm::vec3(4*sin(scale), 0, 0));
	
	// Iterate over all drawables and draw them
	for(size_t i = 1; i < m_pDrawComponents.size(); ++i)
	{
		assert(m_pDrawComponents[i]->m_pTransformComp);
		glUniformMatrix4fv(m_WorldLoc, 1, GL_TRUE, &m_pDrawComponents[i]->m_pTransformComp->GetWorldMatrix()[0][0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_pDrawComponents[i]->m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pDrawComponents[i]->m_IBO);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(DrawComponent::Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(DrawComponent::Vertex), (const GLvoid*)sizeof(glm::vec3));
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pDrawComponents[i]->m_Tex);
		glUniform1i(m_TextureLoc, 0);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	glUseProgram(0);
	
	// Display screen
	glfwSwapBuffers(s_pWindow);
}

