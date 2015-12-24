#pragma once

#include <vector>
#include <unordered_map>
#include "IComponentManager.h"
#include "Base.h"

class Entity;

/**
 * @brief Creates, deletes, and retrieves components of type T
 *
 * Used mainly by the EntityManager. The user will rarely have a need to
 * use any ComponentManager directly.
 *
 * @note Due to the implementation of DeleteFor(entity), components will be moved around
 * at runtime, and therefore the user should NOT store pointers or references to components.
 * This could be changed by having s_CompList store elements of T* rather than T, but further
 * profiling must be done to see if that would have a significant performance effect.
 *
 * @TODO - Profile s_CompList storing T vs storing T*
 */
template <class T>
class ComponentManager : public IComponentManager
{
public:
	virtual ~ComponentManager(){}
	static T* GetFor(EntityID entity);
	static std::vector<T>& GetAll();	//NOTE - We REALLY don't want a user to add or delete elements of this array!
										// @TODO - Create some sort of private array class built over vectors
	static bool CreateFor(EntityID entity, T& component);
	static bool CreateFor(EntityID entity, T&& component);
	virtual void DeleteFor(EntityID entity);
	virtual void DeleteAll();

private:
	static std::vector<std::pair<T, EntityID>> s_CompList;
	static std::unordered_map<EntityID, size_t> s_IDtoIndex;
	static ManagerID s_ID;
};

/**
 * @brief Full specialization of the GUID class so as to allow for the compile-time adding of
 * ComponentManagers to EntityManager::s_pComponentManagers;
 */
template <>
class GUID<IComponentManager, ManagerID>
{
public:
	template <class T>
	static ManagerID GenerateID();

private:
	static void AddManager(IComponentManager* pManager);

	static ManagerID s_CurrentID;
	GUID();
	~GUID();
};

template <class T>
ManagerID GUID<IComponentManager, ManagerID>::GenerateID()
{
	assert(s_CurrentID+1 > s_CurrentID);

	// Add ComponentManager<T> to EntityManager::s_pComponentManagers
	AddManager(new ComponentManager<T>());
	
	return s_CurrentID++;
}

template <class T>
std::vector<std::pair<T, EntityID>> ComponentManager<T>::s_CompList;

template <class T>
std::unordered_map<EntityID, size_t> ComponentManager<T>::s_IDtoIndex;

template <class T>
ManagerID ComponentManager<T>::s_ID = GUID<IComponentManager,ManagerID>::GenerateID<T>();

template <class T>
T* ComponentManager<T>::GetFor(EntityID entity)
{
	assert(s_ID);
	auto iter = s_IDtoIndex.find(entity);
	if(iter == s_IDtoIndex.end())
	{
		return nullptr;
	}

	return &(s_CompList[iter->second].first);
}

template <class T>
std::vector<T>& ComponentManager<T>::GetAll()
{
	assert(s_ID);
	return s_CompList;
}

template <class T>
bool ComponentManager<T>::CreateFor(EntityID entity, T& component)
{
	assert(s_ID);
	if(s_IDtoIndex.find(entity) != s_IDtoIndex.end())
	{
		return false;
	}

	s_IDtoIndex[entity] = s_CompList.size();
	s_CompList.push_back(std::make_pair(component, entity));
	return true;
}

template <class T>
bool ComponentManager<T>::CreateFor(EntityID entity, T&& component)
{
	assert(s_ID);
	if(s_IDtoIndex.find(entity) != s_IDtoIndex.end())
	{
		return false;
	}

	s_IDtoIndex[entity] = s_CompList.size();
	s_CompList.push_back(std::make_pair(component, entity));
	return true;
}

template <class T>
void ComponentManager<T>::DeleteFor(EntityID entity)
{
	assert(s_ID);
	auto iter = s_IDtoIndex.find(entity);
	if(iter == s_IDtoIndex.end())
		return;

	// Swaps the last element with the element to delete, then pops back
	// NOTE: ALL POINTERS TO THE SWAPPED ELEMENT WILL BECOME INVALID AT THIS POINT!!!
	s_IDtoIndex[s_CompList.back().second] = iter->second;
	swap(s_CompList[iter->second], s_CompList.back());
	s_IDtoIndex.erase(iter);
	s_CompList.pop_back();
}

template <class T>
void ComponentManager<T>::DeleteAll()
{
	assert(s_ID);
	s_CompList.clear();
	s_IDtoIndex.clear();

	// Should we get rid of the preallocations after clearing?
}

