#pragma once

#include <unordered_map>
#include "InputConstants.h"

/**
 * @brief Class to perform the mapping of GLFW keys to game events
 *
 * Uses a specified configuration file in a proprietary format to
 * allow the user to indicate which keys should correspond to which
 * game events.
 */
class InputMapper
{
public:
	InputMapper();
	~InputMapper(){}

	/**
	 * @brief Load the specified file to initialize the mapper
	 *
	 * @note This method may be called multiple times; the internal
	 * class structures will be cleared before any new data is loaded.
	 *
	 * @return true unless there was an error with loading or parsing the
	 * configuration file.
	 */
	bool Initialize(std::string filename);

	/**
	 * @return The EGameEvent mapped to the provided key. When the provided key
	 * has no event mapped to it, this method will simply return EGE_NONE.
	 */
	EGameEvent GetEvent(int key) const;

private:
    // Mapping of GLFW key to Event enum
	std::unordered_map<int, EGameEvent> m_KeyMap;
};
