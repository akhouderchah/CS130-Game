#include "ComponentManager.h"
#include "EntityManager.h"

ManagerID GUID<IComponentManager, ManagerID>::s_CurrentID = 1;

void GUID<IComponentManager, ManagerID>::AddManager(IComponentManager* pManager)
{
	EntityManager::s_pComponentManagers.push_back(pManager);
}

