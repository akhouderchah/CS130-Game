#include "InputMapper.h"
#include "ErrorSystem.h"

using namespace std;

#define _Q(x) #x
#define _QUOTE(x) _Q(x)
#define _INSERT(name, comment) std::make_pair(_QUOTE(name), name),

InputMapper::InputMapper()
{
}

bool InputMapper::Initialize(string filename)
{
	// Load in the file
	ifstream infile;
	infile.open(filename);
	if(!infile)
	{
		LOG("Failed to open the keyboard configuration file: " << filename << "\n");
		return false;
	}

	// Clear the key map in case we're initializing an existing InputMapper
	// with a new file
	m_KeyMap.clear();

	// Create map from EGameEvent name to EGameEvent enum value
	unordered_map<string, int> event_map{ EVENTS(_INSERT) };

	// Iterate over the configuration file and popluate key map
	EGameEvent event;
	int lineno = 0;
	std::string line;
	while(getline(infile, line))
	{
		lineno++;

		// Remove the comments (;)
		size_t pch = line.find(';');
		line = line.substr(0, pch);
		if(line == "")
		{
			continue;
		}

		// Parse the mapping of EGameEvent to GLFW key int
		if((pch = line.find('=')))
		{
			string value = line.substr(0,pch);
			int key = atoi((line.substr(pch+1)).c_str());

			// Perform mapping of event name to event enum
			auto event_it = event_map.find(value);
			if(event_it == event_map.end())
			{
				DEBUG_LOG("Keyboard configuration file " << filename <<
						  " has invalid event name: " << value << "\n");
				event = EGE_NONE;
			}
			else
			{
				event = (EGameEvent)event_it->second;
			}

			m_KeyMap.insert(std::make_pair(key,event));
		}
		else
		{
			DEBUG_LOG("Syntax error in keyboard configuration file: " << filename
					  << ":" << lineno << "\n");
		}
	}

	infile.close();
	return true;
}

EGameEvent InputMapper::GetEvent(int key) const
{
	auto iter = m_KeyMap.find(key);
	if(iter == m_KeyMap.end())
	{
		return EGE_NONE;
	}
	return iter->second;
}
