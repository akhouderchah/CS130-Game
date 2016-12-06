#pragma once

#include "IComponent.h"

class DrawSystem;
class TransformComponent;

/**
 * @brief Component to make an entity visible in the game world
 *
 * Provides the draw system with the information required to render
 * an entity. Note that some of this information may come from other
 * components (like the TransformComponent), or from resources external
 * to this component, but nonetheless compiled together in this component.
 */
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

