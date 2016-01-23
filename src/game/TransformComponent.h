#pragma once

#include "../system/IComponent.h"
#include "../base/Base.h"

class TransformComponent : public IComponent
{
public:
	TransformComponent(float x = 0.f, float y = 0.f);
	~TransformComponent();

	void Tick(deltaTime_t dt);

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f position);
	const sf::Vector2f& GetPosition();

private:
	//TransformComponent(const TransformComponent& that);
	//TransformComponent& operator=(const TransformComponent& that);

	// Actual transform data goes here //
	sf::Vector2f m_Position;
};

