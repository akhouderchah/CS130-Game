#pragma once
#include "EntityManager.h"

/**
 * @brief Entity class to be used for all game objects
 *
 * Just a thin wrapper for an ObjHandle (specifically one of type entity).
 * Should not be inherited from. Instead, functionality is added
 * through the creation and modification of components and systems.
 *
 * @note An entity with m_UID.ID == 0 is a null entity. That is, it doesn't
 * in the game, and cannot contain any components. EntityManager::CreateEntity
 * will only create a null entity if no more entities can be created.
 */
class Entity
{
public:
	operator ObjHandle(){ return m_ID; }

	template <class T> T* GetAs()
	{ 
		return EntityManager::GetComponent<T>(m_UID);
	}

	template <class T> inline T* AddComponent(T&& component)
		{
			return EntityManager::AddComponent<T>(*this, std::forward<T>(component));
		}

	template <class T> inline T* AddComponent()
		{
			return EntityManager::AddComponent<T>(*this);
		}
private:
	friend class EntityManager;
	Entity(ObjHandle ID) : m_ID(ID){}

	ObjHandle m_ID;
};

