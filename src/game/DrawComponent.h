#pragma once

#include "../system/IComponent.h"

class DrawSystem;
class TransformComponent;

class DrawComponent : public IComponent
{
public:
	DrawComponent(TransformComponent* pTransform);
	~DrawComponent();

	void Destroy();

private:
	//DrawComponent(const DrawComponent& that);

	// Things that a draw system should know about go here //
	friend DrawSystem;
	TransformComponent* m_pTransformComp;
	GLuint m_VBO;
	GLuint m_IBO;	
};

