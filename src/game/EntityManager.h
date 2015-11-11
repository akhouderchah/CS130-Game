#pragma once
#include <list>
#include <climits>

typedef unsigned short EntityID;
//#define EID_MAX USHRT_MAX
#define EID_MAX 100	// The maximum number of entities allowed

class Entity;

/**
 * @brief Handles all the Entity-related functionality (creation,
 * deletion, adding components, etc)
 */
class EntityManager
{
public:
	static EntityID CreateEntity();
	template <class T> static void AddComponent(EntityID Entity, T Component);
	template <class T> static T& GetComponent(EntityID Entity);
	static void DestroyEntity(Entity& Entity);

private:
	// No need to have EntityManager instances (at least for this project)
	EntityManager();

	// List of open EntityIDs 
	// (stored as inclusive ranges of free numbers)
	static std::list<std::pair<EntityID,EntityID>> s_freeIDs;
};

