#include "EntityManager.h"
#include <climits>
#include "Entity.h"

using namespace std;

// Initialize s_freeIDs with all the available IDs (0 is the null entity)
list<pair<EntityID,EntityID>> EntityManager::s_freeIDs(1,make_pair(1,EID_MAX));

EntityID EntityManager::CreateEntity()
{
	// Take the first free ID, if it exists
	if(!s_freeIDs.empty())
	{
		EntityID ID = s_freeIDs.front().first++;
		if(ID == s_freeIDs.front().second)
		{
			s_freeIDs.pop_front();
		}
		return ID;
	}
	// Otherwise return the null entity and log an error
	// @TODO - Log this as an error!
	return 0;
}

void EntityManager::DestroyEntity(Entity& Entity)
{
	EntityID& UID = Entity.m_UID;
	
	// Add this to s_freeIDs
	// @TODO (performance) temporarily store IDs in an array
	// rather than going through this after every deletion

	UID = 0;
}
