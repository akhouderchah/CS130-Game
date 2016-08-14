/*
  @TODO - ensure we alert components of other types when we delete this component (should be done at the EntityManager level)
*/

#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "IComponentManager.h"
#include "../base/Base.h"
#include "../base/ConstVector.h"
#include "ObjList.h"

class Entity;

/**
 * @brief Creates, deletes, and retrieves components of type T
 *
 * Used mainly by the EntityManager. The user will rarely have a need to
 * use any ComponentManager directly.
 */
template <class T>
class ComponentManager : public IComponentManager
{
public:
	virtual ~ComponentManager(){}
	
	ObjHandle::ID_t Add(IComponent *pComponent)
		{ return m_CompList.Add(pComponent); }
	ObjHandle::ID_t Delete(ObjHandle::ID_t index)
		{ return m_CompList.Delete(index); }
	
	void DeleteAll(){ m_CompList.DeleteAll(); }

	IComponent *Get(ObjHandle::ID_t index) const{ return m_CompList[index]; }
	IComponent *operator[](ObjHandle::ID_t index) const{ return m_CompList[index]; }
	ObjHandle::type_t GetType() const{ return s_ID; }
private:
	ObjList<T> m_CompList;
	static ObjHandle::type_t s_ID;
};

/**
 * @brief Full specialization of the GUID class so as to allow for the compile-time adding of
 * ComponentManagers to EntityManager::s_pComponentManagers;
 */
template <>
class GUID<IComponentManager, ObjHandle::type_t>
{
public:
	template <class T>
	static ObjHandle::type_t GenerateID();

private:
	static void AddManager(IComponentManager* pManager);

	static ObjHandle::type_t s_CurrentID;
	GUID();
	~GUID();
};

template <class T>
ObjHandle::type_t GUID<IComponentManager, ObjHandle::type_t>::GenerateID()
{
	assert(s_CurrentID+1 > s_CurrentID);

	// Add ComponentManager<T> to EntityManager::s_pComponentManagers
	AddManager(new ComponentManager<T>());
	
	return s_CurrentID++;
}

template <class T>
ObjHandle::type_t ComponentManager<T>::s_ID = GUID<IComponentManager,ObjHandle::type_t>::GenerateID<T>();

