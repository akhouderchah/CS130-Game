#pragma once

enum EGameEvent
{
	EGE_NONE = 0,
	EGE_PLAYER1_JUMP,
	EGE_PLAYER2_JUMP,
	EGE_PLAYER3_JUMP,
	EGE_PLAYER4_JUMP,
	EGE_PAUSE,
	EGE_END,
	EGE_ERROR
};

enum EGameState
{
	EGS_DISABLED,
	EGS_STARTED,
	EGS_PAUSED,
	EGS_ENDED
};

struct Event
{
	Event() : event(EGE_NONE), state(EGS_DISABLED){}
	EGameEvent event;
	EGameState state;
};


