#pragma once
#include <vector>
#include "ComponentManager.h"
#include "IComponentManager.h"
#include "Base.h"

//class Entity;
//typedef uint16 EntityID;

/**
 * @brief Handles all the Entity-related functionality (creation,
 * deletion, adding components, etc)
 */
class EntityManager
{
public:
	static void Initialize();
	static void Shutdown();
	static EntityID CreateEntity();
	template <class T> static bool AddComponent(EntityID entity, T& component);
	template <class T> static bool AddComponent(EntityID entity, T&& component);
	template <class T> static T* GetComponent(EntityID entity);
	template <class T> static bool HasComponent(EntityID entity);
	static void DestroyEntity(Entity& Entity);
	static void DestroyAll();

private:
	// No need to have EntityManager instances (at least for this project)
	EntityManager();

	friend class GUID<IComponentManager, ManagerID>;

	// List of open EntityIDs 
	// (stored as inclusive ranges of free numbers)
	static EntityID s_StartID;
	static EntityID s_EndID;
	static std::vector<EntityID> s_freeIDs;
	static const EntityID MAX_IDS;
	static const EntityID FREE_RESERVE;

	static std::vector<IComponentManager*> s_pComponentManagers;
};

template <class T> bool EntityManager::AddComponent(EntityID entity, T& component)
{
	return ComponentManager<T>::CreateFor(entity, component);
}

template <class T> bool EntityManager::AddComponent(EntityID entity, T&& component)
{
	return ComponentManager<T>::CreateFor(entity, component);
}

template <class T> T* EntityManager::GetComponent(EntityID entity)
{
	return ComponentManager<T>::GetFor(entity);
}

template <class T> bool EntityManager::HasComponent(EntityID entity)
{
	return ComponentManager<T>::HasEntity(entity);
}

