#pragma once

#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "IComponentManager.h"

template <class T>
class ComponentManager : public IComponentManager
{
public:
	static T* GetFor(Entity entity);
	static std::vector<T>& GetAll();	//NOTE - We REALLY don't want a user to add or delete elements of this array!
	// @TODO - Create some sort of private array class built over vectors
	static bool CreateFor(Entity entity, T& component);
	static bool CreateFor(Entity entity, T&& component);
	static void DeleteFor(Entity entity);
	static void DeleteAll();

private:
	ComponentManager();

	static std::vector<std::pair<T, EntityID>> s_CompList;
	static std::unordered_map<EntityID, size_t> s_IDtoIndex;
};

template <class T>
std::vector<std::pair<T, EntityID>> ComponentManager<T>::s_CompList;

template <class T>
std::unordered_map<EntityID, size_t> ComponentManager<T>::s_IDtoIndex;

template <class T>
T* ComponentManager<T>::GetFor(Entity entity)
{
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
	return s_CompList;
}

template <class T>
bool ComponentManager<T>::CreateFor(Entity entity, T& component)
{
	if(s_IDtoIndex.find(entity) != s_IDtoIndex.end())
	{
		return false;
	}

	s_IDtoIndex[entity] = s_CompList.size();
	s_CompList.push_back(std::make_pair(component, entity));
	return true;
}

template <class T>
bool ComponentManager<T>::CreateFor(Entity entity, T&& component)
{
	if(s_IDtoIndex.find(entity) != s_IDtoIndex.end())
	{
		return false;
	}

	s_IDtoIndex[entity] = s_CompList.size();
	s_CompList.push_back(std::make_pair(component, entity));
	return true;
}

template <class T>
void ComponentManager<T>::DeleteFor(Entity entity)
{
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
	s_CompList.clear();
	s_IDtoIndex.clear();

	//@TODO - Should we get rid of the preallocations after clearing?
}

