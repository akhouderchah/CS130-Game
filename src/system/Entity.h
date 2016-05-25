#pragma once
#include "EntityManager.h"

/**
 * @brief Entity class to be used for all game objects
 *
 * Just a thin wrapper for an unsigned int of type EntityID.
 * Should not be inherited from. Instead, functionality is added
 * through the creation and modification of components and systems.
 *
 * @note An entity with m_UID == 0 is a null entity. That is, it doesn't
 * exist in memory (and therefore in the game). A null entity will only
 * be created if there are already EID_MAX entities in-game.
 */
class Entity
{
public:
	Entity(EntityID ID) : m_UID(ID){}
	operator EntityID(){ return m_UID; }

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

	EntityID m_UID;
};

