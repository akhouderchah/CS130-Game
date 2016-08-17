#pragma once

#include "Base.h"

class IComponent;

/**
 * @brief Base interface for all component managers.
 *
 * Since there can be an arbitrary number of component managers
 * generated at compile-time, this base class makes it possible
 * to hold a list of what component managers exist.
 */
class IComponentManager
{
	public:
	IComponentManager(){};
	virtual ~IComponentManager(){};

	virtual ObjHandle::ID_t Add(IComponent *pComponent) = 0;

	virtual ObjHandle::ID_t Delete(ObjHandle::ID_t index) = 0;
	virtual void DeleteAll() = 0;

	virtual IComponent *Get(ObjHandle::ID_t index) const = 0;
};

