#include "EntityManager.h"
#include <climits>
#include "Entity.h"

using namespace std;

std::vector<EntityID> EntityManager::s_freeIDs;
const EntityID EntityManager::FREE_RESERVE = 10;
EntityID EntityManager::s_StartID = 1;
EntityID EntityManager::s_EndID = 100;

void EntityManager::Initialize()
{
	s_freeIDs.reserve(FREE_RESERVE);
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

	s_freeIDs.push_back(UID);
	UID = 0;
	
	// @TODO (performance) Perhaps create some sort of
	// unix-like free-list memory scheme?
}
