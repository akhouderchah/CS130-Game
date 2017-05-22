#pragma once

#include "Game.h"
#include "SoundSystem.h"
#include "CollisionSystem.h"

class EventSystem;
class ObserverComponent;

/**
 * @brief Small game to display engine functionality
 */
class TetradGame : public Game
{
public:
	TetradGame();

	virtual bool Initialize(const GameAttributes& attributes);

protected:
	virtual void AddSystems();

private:
	DrawSystem *m_pDrawSystem;
	ObserverComponent *m_pSystemObserver;
	EventSystem *m_pInputSystem;
	SoundSystem *m_pSoundSystem;
	CollisionSystem * m_pCollisionSystem;
};
