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

	// Returns true if the resulting opacity is 0 or 1
	bool SetOpacity(float opacity);
	bool IncrementOpacity(float delta);

	void FadeIn(deltaTime_t fadeTime = 1.f);
	void FadeOut(deltaTime_t fadeTime = 1.f);

	void SetTimeRate(float timeRate){ m_TimeRate = timeRate; }

	void Refresh();

private:
	friend class DrawSystem;
	friend class DrawComponent;
	void Tick(deltaTime_t dt);

private:
	deltaTime_t m_T;
	float m_Opacity;
	float m_Time;
	float m_TimeRate;
};
