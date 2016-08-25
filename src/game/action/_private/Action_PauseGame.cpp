#include "Action_PauseGame.h"

Action_PauseGame::Action_PauseGame(GLFWwindow* pWindow) :
	m_pWindow(pWindow)
{}

bool Action_PauseGame::operator()()
{
	glfwSetWindowShouldClose(m_pWindow, GL_TRUE);
	return true;
}
