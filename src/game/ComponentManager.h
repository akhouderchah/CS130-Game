#pragma once

#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "IComponentManager.h"

template <class T>
class ComponentManager : public IComponentManager
{
public:
	static T& GetFor(Entity entity);
	static std::vector<T>& GetAll();	//NOTE - We REALLY don't want a user to add or delete elements of this array!
	// @TODO - Create some sort of private array class built over vectors
	static void CreateFor(Entity entity, T& component);
	static void CreateFor(Entity entity, T&& component);
	static void DeleteFor(Entity entity);
	static void DeleteAll();

private:
	ComponentManager();

	static std::vector<T> s_CompList;
	static std::unordered_map<EntityID, size_t> s_IDtoIndex;
};

template <class T>
std::vector<T> ComponentManager<T>::s_CompList;

template <class T>
std::unordered_map<EntityID, size_t> ComponentManager<T>::s_IDtoIndex;

template <class T>
T& ComponentManager<T>::GetFor(Entity entity)
{
	return s_CompList[s_IDtoIndex[entity]];
}

template <class T>
std::vector<T>& ComponentManager<T>::GetAll()
{
	return s_CompList;
}

template <class T>
void ComponentManager<T>::CreateFor(Entity entity, T& component)
{
	s_IDtoIndex[entity] = s_CompList.size();
	s_CompList.push_back(component);
}

template <class T>
void ComponentManager<T>::CreateFor(Entity entity, T&& component)
{
	s_IDtoIndex[entity] = s_CompList.size();
	s_CompList.push_back(component);
}

template <class T>
void ComponentManager<T>::DeleteFor(Entity entity)
{
	if(s_IDtoIndex.find(entity) != s_IDtoIndex.end())
	{
		// @TODO - PERFORMANCE
		s_CompList.erase(s_CompList.begin() + s_IDtoIndex[entity]);
		s_IDtoIndex.erase(entity);
	}
}

template <class T>
void ComponentManager<T>::DeleteAll()
{
	s_CompList.clear();
	s_IDtoIndex.clear();

	//@TODO - Should we get rid of the preallocations after clearing?
}

