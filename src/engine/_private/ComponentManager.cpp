#include "ComponentManager.h"
#include "EntityManager.h"

ObjHandle::type_t GUID<IComponentManager, ObjHandle::type_t>::s_CurrentID = 0;

void GUID<IComponentManager, ObjHandle::type_t>::AddManager(IComponentManager* pManager)
{
	EntityManager::s_pComponentManagers.push_back(pManager);
	std::cout << EntityManager::s_pComponentManagers.size() << std::endl;
}

