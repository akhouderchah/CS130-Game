#include "Action_PauseGame.h"
#include "MaterialComponent.h"
#include "SoundComponent.h"
Entity Action_PauseGame::s_FadeEntity = nullEntity;
Entity Action_PauseGame::s_BackGroundEntity= nullEntity;
bool Action_PauseGame::s_bFadeOut = false;

Action_PauseGame::Action_PauseGame(GLFWwindow* pWindow) :
	m_pWindow(pWindow)
{}

bool Action_PauseGame::operator()()
{
	//glfwSetWindowShouldClose(m_pWindow, GL_TRUE);

	MaterialComponent *fader = EntityManager::GetComponent<MaterialComponent>(s_FadeEntity);
	SoundComponent *pSnd = s_BackGroundEntity.GetAs<SoundComponent>();
	if(s_bFadeOut)
	{
		fader->FadeOut(.25f);
		pSnd->PlaySound("backgroundMusic");
	}
	else
	{
		fader->FadeIn(.5f);
		pSnd->PauseSound("backgroundMusic");
	}

	s_bFadeOut = !s_bFadeOut;
	return true;
}
