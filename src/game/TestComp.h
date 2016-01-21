#ifndef TEST_H
#define TEST_H

#include <iostream>

#include "IComponent.h"

class TestComponent : public IComponent
{
public:
	virtual void Tick(deltaTime_t dt){ (void)dt; std::cout << "Tick!\n"; }
};

#endif //TEST_H
