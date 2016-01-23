#pragma once

#include "IComponent.h"

struct Event
{

};

class ObserverComponent : public IComponent
{
public:
	ObserverComponent();
	~ObserverComponent();

	bool Tick(const Event& event);

};

