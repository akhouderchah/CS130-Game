#pragma once

#include "Base.h"
#include "EntityManager.h"

/**
 * @brief Base class for all components
 */
class IComponent
{
public:
	virtual ~IComponent(){}

	/**
	 * @brief Function to refresh ptrs to other components
	 *
	 * In order to keep the entity system efficent, components will cache
	 * pointers to components they need. This method provides the entity
	 * manager with the ability to clear these cached pointers as needed
	 * and force components to use the EntityManager to re-acquire other
	 * component pointers.
	 */
	virtual void Refresh() = 0;

	Entity GetEntity() const{ return m_Entity; }
	ObjHandle::ID_t GetID() const{ return ((ObjHandle)m_Entity).GetID(); }

protected:
	IComponent(Entity entity) : m_Entity(entity){}

	Entity m_Entity;
};

