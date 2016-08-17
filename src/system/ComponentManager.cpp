#include "ComponentManager.h"
#include "EntityManager.h"

EntityManager eMan;

ObjHandle::type_t GUID<IComponentManager, ObjHandle::type_t>::s_CurrentID = 0;

void GUID<IComponentManager, ObjHandle::type_t>::AddManager(IComponentManager* pManager)
{
	eMan.s_pComponentManagers.push_back(pManager);
	std::cout << eMan.s_pComponentManagers.size() << std::endl;
}

