#pragma once

enum EGameEvent
{
	EGE_NONE = -1,      // No Observer reacts to this event. Effectively a "null event".
	EGE_PLAYER1_JUMP,  // Tell Player1 to jump
	EGE_PLAYER2_JUMP,  // Tell Player2 to jump
	EGE_PLAYER3_JUMP,  // Tell Player3 to jump
	EGE_PLAYER4_JUMP,  // Tell Player4 to jump
	EGE_PAUSE,         // Pause the game. This event can be captured by the "system observer", as well as by any components that need special behavior when paused.
	EGE_ERROR,          // Event (mainly for the system observer) to signal that something bad happened.
	EGE_END           // Event that the EventQueue is empty. Can also be used as the size of an array containing the different Events.
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


