#pragma once

#include "IComponent.h"

class DrawSystem;
class TransformComponent;

class DrawComponent : public IComponent
{
public:
	DrawComponent(Entity entity);
	~DrawComponent();

	void Refresh();
	
private:
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 uv;
	};
	//DrawComponent(const DrawComponent& that);

	// Things that a draw system should know about go here //
	friend DrawSystem;
	TransformComponent* m_pTransformComp;
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_Tex;
};

