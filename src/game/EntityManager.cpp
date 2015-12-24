#include "EntityManager.h"
#include <climits>
#include "Entity.h"

using namespace std;

/**
 * EntityManager Static definitions
 */
std::vector<EntityID> EntityManager::s_freeIDs;
const EntityID EntityManager::FREE_RESERVE = 10;
EntityID EntityManager::s_StartID = 1;
EntityID EntityManager::s_EndID = 100;
std::vector<IComponentManager*> EntityManager::s_pComponentManagers;
/**/

void EntityManager::Initialize()
{
	s_freeIDs.reserve(FREE_RESERVE);
}

void EntityManager::Shutdown()
{
	s_freeIDs.clear();

	size_t i;
	for(i = 0; i < s_pComponentManagers.size(); ++i)
	{
		s_pComponentManagers[i]->DeleteAll();
	}

	i = s_pComponentManagers.size();

	while(i != 0)
	{
		delete s_pComponentManagers[--i];
		s_pComponentManagers.pop_back();
	}
}

EntityID EntityManager::CreateEntity()
{
	// Take the first free ID, if it exists
	if(!s_freeIDs.empty())
	{
		EntityID ID = s_freeIDs.back();
		s_freeIDs.pop_back();
		return ID;
	}
	// Using Start-1 because EndID == USHRT_MAX would cause an
	// overflow eventually
	else if((s_StartID-1) < s_EndID)
	{
		return s_StartID++;
	}
	
	// Otherwise return the null entity and log an error
	// @TODO - Log this as an error!
	return 0;
}

void EntityManager::DestroyEntity(Entity& Entity)
{
	EntityID& UID = Entity.m_UID;

	for(size_t i = 0; i < s_pComponentManagers.size(); ++i)
	{
		s_pComponentManagers[i]->DeleteFor(Entity);
	}

	s_freeIDs.push_back(UID);
	UID = 0;
	
	// @TODO (performance) Perhaps create some sort of
	// unix-like free-list memory scheme?
}

