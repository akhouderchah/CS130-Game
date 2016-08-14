#pragma once

#include "../base/Base.h"

/*
 * @brief Base class for all components
 */
class IComponent
{
public:
	virtual ~IComponent(){}

	virtual bool Init() = 0;
	virtual void Destroy() = 0;
	//virtual void Tick(deltaTime_t dt) = 0;

	ObjHandle::ID_t GetID() const{ return m_ID; }

protected:
	IComponent(ObjHandle::ID_t ID) : m_ID(ID){}

private:
	ObjHandle::ID_t m_ID;
};

