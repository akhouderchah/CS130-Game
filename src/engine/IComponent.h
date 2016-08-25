#pragma once

#include "Base.h"
#include "EntityManager.h"

/*
 * @brief Base class for all components
 */
class IComponent
{
public:
	virtual ~IComponent(){}
	
	virtual void Refresh() = 0;  // Function to refresh ptrs to other components
	//virtual void Tick(deltaTime_t dt) = 0;

	Entity GetEntity() const{ return m_Entity; }
	ObjHandle::ID_t GetID() const{ return ((ObjHandle)m_Entity).GetID(); }

protected:
	IComponent(Entity entity) : m_Entity(entity){}

	Entity m_Entity;
};

