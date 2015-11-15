#pragma once
#include <vector>
#include "ComponentManager.h"
#include "Entity.h"

/**
 * @brief Handles all the Entity-related functionality (creation,
 * deletion, adding components, etc)
 */
class EntityManager
{
public:
	static void Initialize();
	static EntityID CreateEntity();
	template <class T> static void AddComponent(Entity entity, T& component);
	template <class T> static void AddComponent(Entity entity, T&& component);
	template <class T> static T& GetComponent(Entity entity);
	static void DestroyEntity(Entity& Entity);

private:
	// No need to have EntityManager instances (at least for this project)
	EntityManager();

	// List of open EntityIDs 
	// (stored as inclusive ranges of free numbers)
	static EntityID s_StartID;
	static EntityID s_EndID;
	static std::vector<EntityID> s_freeIDs;
	static const EntityID FREE_RESERVE;
};

template <class T> void EntityManager::AddComponent(Entity entity, T& component)
{
	ComponentManager<T>::CreateFor(entity, component);
}

template <class T> void EntityManager::AddComponent(Entity entity, T&& component)
{
	ComponentManager<T>::CreateFor(entity, component);
}

template <class T> T& EntityManager::GetComponent(Entity entity)
{
	return ComponentManager<T>::GetFor(entity);
}

