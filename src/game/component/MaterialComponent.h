#pragma once

#include "IComponent.h"

class DrawComponent;

/**
 * @brief Component to allow changes in material effects at runtime
 *
 * Current effects supported are:
 *    + Fade in/out
 */
class MaterialComponent : public IComponent
{
public:
	MaterialComponent(Entity entity);
	~MaterialComponent();

	void FadeIn(deltaTime_t fadeTime = 1.f);
	void FadeOut(deltaTime_t fadeTime = 1.f);

	void Refresh();

private:
	friend class DrawSystem;
	void Tick(deltaTime_t dt);

private:
	DrawComponent *m_pDrawComp;

	deltaTime_t m_T;
};
