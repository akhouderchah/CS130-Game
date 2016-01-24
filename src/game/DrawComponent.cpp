#include "DrawComponent.h"

DrawComponent::DrawComponent()
{
}

DrawComponent::~DrawComponent()
{
}

void DrawComponent::Destroy()
{
	for(size_t i = m_pDrawables.size(); i > 0; )
	{
		delete m_pDrawables[--i];
	}
}

