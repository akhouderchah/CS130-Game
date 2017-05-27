#pragma once

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>

#define EVENTS(f)														\
    f(NONE, "No Observer reacts to this event. Serves as a null event")	\
    f(PAUSE, "Pause the game. This event can be captured by the system observer as well as by any components that need special behavior when paused.") \
    f(PLAYER1_JUMP, "Tell Player1 to jump")								\
	f(PLAYER1_RIGHT, "Tell Player1 to go right")                        \
	f(PLAYER1_LEFT, "Tell Player1 to go left")                          \
    f(PLAYER2_JUMP, "Tell Player2 to jump")								\
	f(TOGGLE_HITBOX_VIEW, "Toggles the hitbox view on and off")			\
    f(ERROR, "Event (mainly for the system observer) to signal errors.")

#define _EVENT_ENUM(name, comment) EGE_ ## name,
enum EGameEvent{ EVENTS(_EVENT_ENUM) EGE_END };


enum EGameState
{
	EGS_DISABLED,
	EGS_STARTED,
	EGS_PAUSED,
	EGS_ENDED
};

/**
 * @brief Structure containing information for a game event
 *
 * Contains information indicating what the new state for a
 * particular game event is. Used mostly by the EventSystem
 * and ObserverComponents.
 */
struct Event
{
	Event() : event(EGE_NONE), state(EGS_DISABLED){}
	EGameEvent event;
	EGameState state;
};


