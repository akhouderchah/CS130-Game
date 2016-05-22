#pragma once

#include "../system/IComponent.h"

class DrawSystem;

class DrawComponent : public IComponent
{
public:
	DrawComponent();
	~DrawComponent();

	void Destroy();

private:
	//DrawComponent(const DrawComponent& that);

	// Things that a draw system should know about go here //
	friend DrawSystem;
	glm::mat4 m_World;
	GLuint m_VBO;
	GLuint m_IBO;	
};

