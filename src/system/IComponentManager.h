#pragma once

#include "Base.h"

/**
 * @brief Base interface for all component managers.
 *
 * Since there can be an arbitrary number of component managers
 * generated at compile-time, this base class makes it easy to
 * hold a list of what component managers exist.
 *
 * Although all ComponentManagers have the same basic interface,
 * placing a function in the IComponentManager prevents it
 * from being static. Thus, only functions that must be applied to a
 * ComponentManager<T> of unknown T at compile-time should be placed here.
 */
class IComponentManager
{
	public:
	// Whatever interface will be needed.
	// Perhaps some sort of event-handling functions.
	IComponentManager(){};
	virtual ~IComponentManager(){};
	virtual void DeleteFor(EntityID entity) = 0;
	virtual void DeleteAll() = 0;
};

