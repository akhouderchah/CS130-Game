#pragma once

#include "../system/IComponent.h"

class DrawComponent : public IComponent
{
public:
	DrawComponent();
	~DrawComponent();

	void Destroy();

	//std::vector<sf::Drawable*>& GetDrawables(){ return m_pDrawables; }
	//bool AddDrawable(sf::Drawable* pDrawable){ m_pDrawables.push_back(pDrawable); return true;}

private:
	//DrawComponent(const DrawComponent& that);

	// Things that a draw system should know about go here //
	//std::vector<sf::Drawable*> m_pDrawables;
};

