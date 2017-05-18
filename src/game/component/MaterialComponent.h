#pragma once

#include "IComponent.h"

class DrawComponent;

/**
 * @brief Component to allow changes in material effects at runtime
 *
 * Current effects supported are:
 *    + Fade in/out
 *    + Texture scrolling
 */
class MaterialComponent : public IComponent
{
public:
	MaterialComponent(Entity entity);
	~MaterialComponent();

	/**
	 * @brief Set the transparency of the entity to a value between 0 and 1
	 *
	 * @return true if the resulting opacity is 0 or 1
	 */
	bool SetOpacity(float opacity);

	/**
	 * @brief Add modify the transparency of the entity by some amount
	 *
	 * @note Opacity will be clamped between 0 and 1
	 * @return true if the resulting opacity is 0 or 1
	 */
	bool IncrementOpacity(float delta);

	/**
	 * @brief Change opacity from current value to 1
	 */
	void FadeIn(deltaTime_t fadeTime = 1.f);

	/**
	 * @brief Change opacity from current value to 0
	 */
	void FadeOut(deltaTime_t fadeTime = 1.f);

	/**
	 * @brief Set the texture scroll rate
	 */
	void SetScrollRate(float scrollRate){ m_ScrollRate = scrollRate; }

	void Refresh();

private:
	friend class DrawSystem;
	friend class DrawComponent;
	void Tick(deltaTime_t dt);

private:
	deltaTime_t m_T;
	float m_Opacity;
	float m_Time;
	float m_ScrollRate;
};
