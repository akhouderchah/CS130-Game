#pragma once

#include "Base.h"
#include "ConstVector.h"
#include "IComponentManager.h"
#include "ObjList.h"

#include <iostream>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

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
	ConstVector<T*> GetAll(){ return m_CompList.GetAll(); }
	IComponent *operator[](ObjHandle::ID_t index) const{ return m_CompList[index]; }
	static ObjHandle::type_t GetType(){ return s_ID; }
private:
	ComponentManager(){ std::cout << "Creating manager of type: " << typeid(T).name() << std::endl; }
	friend class GUID<IComponentManager, ObjHandle::type_t>;

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

template <typename T>
ObjHandle::type_t GUID<IComponentManager, ObjHandle::type_t>::GenerateID()
{
	assert(s_CurrentID+1 > s_CurrentID);

	// Add ComponentManager<T> to EntityManager::s_pComponentManagers
	AddManager(new ComponentManager<T>());

	return s_CurrentID++;
}

template <class T>
ObjHandle::type_t ComponentManager<T>::s_ID = GUID<IComponentManager,ObjHandle::type_t>::GenerateID<T>();

