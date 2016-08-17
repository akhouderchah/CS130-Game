#pragma once

#include "../system/IComponent.h"

class DrawSystem;
class TransformComponent;

class DrawComponent : public IComponent
{
public:
	DrawComponent(Entity entity);
	~DrawComponent();

	void Refresh();
	
private:
	//DrawComponent(const DrawComponent& that);

	// Things that a draw system should know about go here //
	friend DrawSystem;
	TransformComponent* m_pTransformComp;
	GLuint m_VBO;
	GLuint m_IBO;	
};

