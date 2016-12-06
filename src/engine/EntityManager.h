#pragma once

#include "Base.h"
#include "ComponentManager.h"

#include <queue>
#include <vector>

class EntityManager;

/**
 * @brief Entity class to be used for all game objects
 *
 * Just a thin wrapper for an ObjHandle (specifically one of type entity).
 * Should not be inherited from. Instead, functionality is added
 * through the creation and modification of components and systems.
 *
 * @note An entity with m_UID.ID == 0 is a null entity. That is, it doesn't exist
 * in the game, and cannot contain any components. EntityManager::CreateEntity
 * will only create a null entity if no more entities can be created.
 */
class Entity
{
public:
	Entity(ObjHandle ID=ObjHandle::null) : m_ID(ID){} /** @TODO - MOVE BACK TO PRIVATE!!! */
	operator ObjHandle() const{ return m_ID; }
	bool operator ==(Entity other) const{ return (other.m_ID.GetID() == m_ID.GetID()) && (other.m_ID.GetVersion() == m_ID.GetVersion()); }
	bool operator !=(Entity other) const{ return !(other == *this); }

	template <typename T> inline T *GetAs();
	template <typename T> inline T *Add();

private:
	friend class EntityManager;

	ObjHandle m_ID;
};

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
	/**
	 * @brief Initialization function for the entity manager.
	 *
	 * MUST be called before any other EntityManager calls are made.
	 */
	static void Initialize();
	
	/**
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
	
	template <class T> static T *AddComponent(Entity entity, bool skipRefresh=false);
	template <class T> static T *GetComponent(Entity entity);
	template <class T> static ConstVector<T*> GetAll();
	template <class T> static bool HasComponent(Entity entity);
	template <class T> static void RemoveComponent(Entity entity, bool skipRefresh=false);

	static IComponent *GetComponent(Entity entity, ObjHandle::type_t type);
	static bool HasComponent(Entity entity, ObjHandle::type_t type);
	static void RemoveComponent(Entity entity, ObjHandle::type_t type, bool skipRefresh=false);

private:
	// No need to have EntityManager instances (at least for this project)
	//	EntityManager();

	static IComponent *AddComponent(Entity entity, ObjHandle::type_t, IComponent *pComp, bool skipRefresh=false);
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
T *EntityManager::AddComponent(Entity entity, bool skipRefresh)
{
	ObjHandle::type_t type = ComponentManager<T>::GetType();
	T *pComp = new T(entity);
	
	return (T*)AddComponent(entity, type, pComp, skipRefresh);
}

template <typename T>
T *EntityManager::GetComponent(Entity entity)
{
	ObjHandle::type_t type = ComponentManager<T>::GetType();

	return (T*)GetComponent(entity, type);
}

template <typename T>
ConstVector<T*> EntityManager::GetAll()
{
	return ((ComponentManager<T>*)s_pComponentManagers[ComponentManager<T>::GetType()])->GetAll();
}

template <typename T> bool EntityManager::HasComponent(Entity entity)
{
	return HasComponent(entity, ComponentManager<T>::GetType());
}

template <typename T> void EntityManager::RemoveComponent(Entity entity, bool skipRefresh)
{
	return RemoveComponent(entity, ComponentManager<T>::GetType(), skipRefresh);
}

template <typename T> T *Entity::GetAs()
{
	return EntityManager::GetComponent<T>(*this);
}

template <typename T> T *Entity::Add()
{
	return EntityManager::AddComponent<T>(*this);
}

