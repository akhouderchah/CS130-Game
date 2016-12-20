#pragma once

#include "Config.h"
#include "Constants.h"
#include "ErrorSystem.h"
#include "LogSystem.h"
#include "Platform.h"
#include "Paths.h"
#include "Rand.h"
#include "Types.h"

#include "GL/glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"

#ifdef WIN32
//#include <Windows.h>
#endif

#include <cassert>

const std::string VERSION_STRING = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR);

/** System Globals **/
class Entity;
extern const Entity nullEntity;

/**
 * @brief Determines what data structures should do when they
 * are filled.
 */
enum EOverflowBehavior
{
	EOB_Replace_Oldest,		// Replace the oldest element with the current one
	EOB_Replace_Youngest,	// Replace the youngest element with the current one
	EOB_Ignore				// Ignore the current element
};

/**
 * @brief Set of classes to generate GUIDs for different purposes.
 *
 * @in - T - The GUID "purpose" (the set of classes (base class perhaps) that will use the GUID)
 * @in - IDType - The data type of the ID (most likely an integer type, although it could be specialized
 *	different types)
 *
 *	@note The base implementation only generates IDs, it does not reclaim them, even if an ID is no longer used.
 */
template <class T, class IDType>
class GUID
{
public:
	static IDType GenerateID()
		{
			assert(s_currentID+1 > s_currentID);	// Check for overflow
			return s_currentID++;
		}

private:
	static IDType s_currentID;
	GUID();
	~GUID();
};

