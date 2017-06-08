#pragma once

#include "IAction.h"

/**
 * @brief System observer action to pause the game
 */
class Action_PauseGame : public IAction
{
public:
	Action_PauseGame(GLFWwindow* pWindow);
	virtual ~Action_PauseGame(){}

	static void SetFadeScreen(Entity entity){ s_FadeEntity = entity; }
	static void SetBackGround(Entity entity){
		s_BackGroundEntity = entity;
	}
	virtual bool operator()();

private:
	GLFWwindow* m_pWindow;
	static Entity s_FadeEntity;
	static Entity s_BackGroundEntity;
	static bool s_bFadeOut;
};
