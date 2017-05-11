#pragma once

#include "Base.h"
#include "IComponent.h"

class MovableComponent;
class DrawComponent;


class CollisionComponent : public IComponent
{
public:
	CollisionComponent(Entity entity);

	bool init(const glm::vec3&, const glm::vec3&);

	void Refresh();

	void Tick(deltaTime_t dt);

	glm::vec3 &  getHitboxPosition();
	glm::vec3 &  getHitboxScale();

	void createEntities();
	void moveBox(glm::vec3);

	void toggleHitboxView();

private:
	DrawComponent * m_pDraw;
	MovableComponent* m_pMover;
	glm::vec3 m_HitboxPosition;
	glm::vec3 m_HitboxScale;

	Entity entity;
	bool isCollisionViewEnabled;
};
