#include "Action_PauseGame.h"
#include "MaterialComponent.h"

Entity Action_PauseGame::s_FadeEntity = nullEntity;
bool Action_PauseGame::s_bFadeOut = false;

Action_PauseGame::Action_PauseGame(GLFWwindow* pWindow) :
	m_pWindow(pWindow)
{}

bool Action_PauseGame::operator()()
{
	//glfwSetWindowShouldClose(m_pWindow, GL_TRUE);

	MaterialComponent *fader = EntityManager::GetComponent<MaterialComponent>(s_FadeEntity);
	if(s_bFadeOut)
	{
		fader->FadeOut(.5f);
	}
	else
	{
		fader->FadeIn(.5f);
	}
	s_bFadeOut = !s_bFadeOut;
	return true;
}
