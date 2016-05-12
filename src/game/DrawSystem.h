#pragma once

#include "../system/ISystem.h"
#include "DrawComponent.h"
#include "TransformComponent.h"
#include "../base/ConstVector.h"
#include "ShaderProgram.h"

class DrawSystem : public ISystem
{
public:
	DrawSystem();
	virtual ~DrawSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);

private:
	ConstVector<std::pair<DrawComponent, EntityID>> m_DrawComponents;
	ConstVector<std::pair<TransformComponent, EntityID>> m_TransformComponents;
	GLuint m_Program;
	GLuint VBO;
	GLuint IBO;
};

