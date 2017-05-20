#pragma once

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>

#define EVENTS(f)														          \
    f(EGE_NONE, "No Observer reacts to this event. Serves as a null event")       \
    f(EGE_PAUSE, "Pause the game. This event can be captured by the system observer as well as by any components that need special behavior when paused.")	           \
    f(EGE_PLAYER1_JUMP, "Tell Player1 to jump")							          \
    f(EGE_PLAYER2_JUMP, "Tell Player2 to jump")							          \
    f(EGE_ERROR, "Event (mainly for the system observer) to signal errors.")

#define _EVENT_ENUM(name, comment) name,
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


