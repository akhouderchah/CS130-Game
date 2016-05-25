#pragma once

#include "../system/ISystem.h"
#include "DrawComponent.h"
#include "TransformComponent.h"
#include "MovableComponent.h"
#include "CameraComponent.h"
#include "../base/ConstVector.h"
#include "ShaderProgram.h"

class DrawSystem : public ISystem
{
public:
	DrawSystem();
	virtual ~DrawSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	void SetCurrentCamera(CameraComponent* pCamera);
	virtual void Tick(deltaTime_t dt);

private:
	ConstVector<std::pair<DrawComponent*, EntityID>> m_pDrawComponents;
	CameraComponent m_CurrentCamera;
	GLuint m_Program;
	GLuint m_WorldLoc;
};

