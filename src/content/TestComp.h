#ifndef TEST_H
#define TEST_H

#include <iostream>

#include "Component.h"

class TestComponent : public Component
{
public:
	virtual void Tick(){ std::cout << "Tick!\n"; }
};

#endif //TEST_H
