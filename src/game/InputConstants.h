#pragma once
#ifndef _INPUTCONSTANTS_H
#define _INPUTCONSTANTS_H
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 200
#endif

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <map>

static std::map<int,std::string> actions;
static std::map<std::string,int> event_map;
static void init_event_map()
{
	event_map.insert(std::make_pair("EGE_NONE",0));
	event_map.insert(std::make_pair("EGE_PLAYER1_JUMP",1));
	event_map.insert(std::make_pair("EGE_PLAYER2_JUMP",2));
	event_map.insert(std::make_pair("EGE_PLAYER3_JUMP",3));
	event_map.insert(std::make_pair("EGE_PLAYER4_JUMP",4));
	event_map.insert(std::make_pair("EGE_PAUSE",5));
	event_map.insert(std::make_pair("EGE_ERROR",6));
	event_map.insert(std::make_pair("EGE_END",7));
};

static void INIParser(std::string filename)
{
	std::ifstream infile;
	std::string line;
	std::string key;
	int value;
	int lineno=0;
	std::string KEYBOARD_PATH=KEY_PATH+filename;//ini file in /asset/config folder
	infile.open(KEYBOARD_PATH);
	int error=0;
	//assert(infile.is_open());

	while ( std::getline(infile, line))
	{
		lineno++;
		size_t pch = line.find(';');//remove the comments
		if (pch) 
			line= line.substr(0,pch);
		if (pch = line.find(':'))
		{
			key = line.substr(0,pch);
			value = atoi((line.substr(pch+1)).c_str());
			actions.insert(std::make_pair(value,key));
		}
		else 
		{
			error=lineno;
		}
	}
	
	infile.close();
	
}




enum EGameEvent
{
	EGE_NONE = 0,      // No Observer reacts to this event. Effectively a "null event".
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
#endif

