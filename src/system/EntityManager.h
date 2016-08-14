#pragma once
#include <vector>
#include <queue>
#include "Base.h"
#include "ComponentManager.h"
#include "IComponentManager.h"
#include "Entity.h"

/**
 * @brief Handles all the Entity-related functionality (creation,
 * deletion, adding components, etc)
 *
 * Entity 0 is created at initialization, and serves as the null entity.
 * Entity 0 will always fail to add or get components (returning nullptr).
 * Neither DestroyEntity nor DestroyAll will remove the null entity element.
 */
class EntityManager
{
public:
	/*
	 * @brief Initialization function for the entity manager.
	 *
	 * MUST be called before any other EntityManager calls are made.
	 */
	static void Initialize();
	
	/*
	 * @brief Shutdown function for the entity manager.
	 *
	 * @note The EntityManager cannot be restarted or used after calling
	 * this function. Therefore this should only occur on engine destruction
	 * (or on an otherwise permanent engine shutdown). If one simply wants to
	 * "reset" the EntityManager, call DestroyAll().
	 */
	static void Shutdown();

	static Entity CreateEntity();
	static void DestroyEntity(Entity entity);
	static void DestroyAll();
	
	template <class T> static T *AddComponent(Entity entity);
	template <class T> static T *GetComponent(Entity entity);
	template <class T> static bool HasComponent(Entity entity);
	template <class T> static void RemoveComponent(Entity entity);
	static bool HasComponent(Entity entity, ObjHandle::type_t type);
	static void RemoveComponent(Entity entity, ObjHandle::type_t type);

private:
	// No need to have EntityManager instances (at least for this project)
	EntityManager();

	static void AddEntities(size_t chunkSize = EntityManager::CHUNK_SIZE);
	
private:
	friend class GUID<IComponentManager, ObjHandle::type_t>;
	static std::vector<IComponentManager*> s_pComponentManagers;

	typedef std::vector<std::pair<ObjHandle::type_t, ObjHandle::ID_t>> compList_t;
	static std::vector<std::pair<ObjHandle::version_t, compList_t>> s_EntityList;
	static std::queue<ObjHandle::ID_t> s_FreeList;

	// Maps ID:type -> component index
	static std::unordered_map<ObjHandle::handle_t, ObjHandle::ID_t> s_HandletoIndex;

	static const size_t CHUNK_SIZE;
};

template <typename T>
T *EntityManager::AddComponent(Entity entity)
{
	ObjHandle::ID_t index = entity.m_ID.GetID();
	ObjHandle::type_t type = ComponentManager<T>::GetType();
	
	// Ensure the index and version are valid
	if(index == 0 || index >= s_EntityList.size() ||
	   entity.m_ID.GetVersion() != s_EntityList[index].first)
	{
		return s_pComponentManagers[type]->Get(0);
	}

	// Get space in actual component manager
	T *pComp = new T(index);
	ObjHandle::ID_t compIndex = s_pComponentManagers[type]->Add(pComp);
	T *pActual = *s_pComponentManagers[type]->Get(compIndex);
	if(pActual != pComp)
	{
		delete pComp;
		return pActual; // Don't set data structures if the add failed
	}

	// Add to s_EntityList
	s_EntityList[index].second.push_back(std::make_pair(type, compIndex));

	// Add to s_HandletoIndex
	ObjHandle::handle_t handle = ObjHandle::constructRawHandle(index, type, 0u);
	s_HandletoIndex[handle] = compIndex;

	return pActual;
}

template <typename T>
T *EntityManager::GetComponent(Entity entity)
{
	ObjHandle::ID_t ID = entity.m_ID.GetID();
	ObjHandle::type_t type = ComponentManager<T>::GetType();

	// Validity tests
	if(ID == 0 || ID >= s_EntityList.size() ||
	   entity.m_ID.GetVersion() != s_EntityList[ID].first)
	{
		return s_pComponentManagers[type]->Get(0);
	}
	
	// Get component
	ObjHandle::handle_t handle = ObjHandle::constructRawHandle(ID, type, 0u);
	auto iter = s_HandletoIndex.find(handle);
	if(iter == s_HandletoIndex.end())
	{
		// Return null if entity doesn't have that component type
		return s_pComponentManagers[type]->Get(0);
	}
	ObjHandle::ID_t compIndex = iter->second;
	
	return s_pComponentManagers[type]->Get(compIndex);
}

template <typename T> bool EntityManager::HasComponent(Entity entity)
{
	return HasComponent(entity, ComponentManager<T>::GetType());
}

template <typename T> void EntityManager::RemoveComponent(Entity entity)
{
	return RemoveComponent(entity, ComponentManager<T>::GetType());
}
