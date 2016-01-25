#pragma once

#include "../system/IComponent.h"
#include "../base/Base.h"

class TransformComponent : public IComponent
{
public:
	TransformComponent(float x = 0.f, float y = 0.f);
	~TransformComponent();

	void Tick(deltaTime_t dt);
	void Destroy(){}

	void SetPosition(float x, float y);
	void SetPosition(const glm::vec2& position);
	const glm::vec2& GetPosition();

private:
	//TransformComponent(const TransformComponent& that);
	//TransformComponent& operator=(const TransformComponent& that);

	// Actual transform data goes here //
	glm::vec2 m_Position;
};

