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

	virtual bool operator()();
	
private:
	GLFWwindow* m_pWindow;
};
