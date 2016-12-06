#include "Base.h"
#include "EntityManager.h"

const Entity nullEntity(ObjHandle::constructHandle(0,0,0));

/**
 * GUID class family static definitions
 */
template <class T, class IDType>
IDType GUID<T, IDType>::s_currentID = 1;

