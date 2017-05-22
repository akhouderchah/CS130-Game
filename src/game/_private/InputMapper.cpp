#include "Base.h"
#include "InputMapper.h"
#include "ErrorSystem.h"

using namespace std;

#define _Q(x) #x
#define _QUOTE(x) _Q(x)
#define _INSERT(name, comment) std::make_pair(_QUOTE(name), EGE_ ## name),

#define _KEY_INSERT(name) std::make_pair(_QUOTE(name), GLFW_KEY_ ## name),
#define KEYS(f)									\
	f(SPACE)									\
	f(APOSTROPHE)								\
	f(COMMA)									\
	f(MINUS)									\
	f(PERIOD)									\
	f(SLASH)									\
	f(0)										\
	f(1)										\
	f(2)										\
	f(3)										\
	f(4)										\
	f(5)										\
	f(6)										\
	f(7)										\
	f(8)										\
	f(9)										\
	f(SEMICOLON)								\
	f(EQUAL)									\
	f(A)										\
	f(B)										\
	f(C)										\
	f(D)										\
	f(E)										\
	f(F)										\
	f(G)										\
	f(H)										\
	f(I)										\
	f(J)										\
	f(K)										\
	f(L)										\
	f(M)										\
	f(N)										\
	f(O)										\
	f(P)										\
	f(Q)										\
	f(R)										\
	f(S)										\
	f(T)										\
	f(U)										\
	f(V)										\
	f(W)										\
	f(X)										\
	f(Y)										\
	f(Z)										\
	f(LEFT_BRACKET)								\
	f(BACKSLASH)								\
	f(RIGHT_BRACKET)							\
	f(GRAVE_ACCENT)								\
	f(WORLD_1)									\
	f(WORLD_2)									\
	f(ESCAPE)									\
	f(ENTER)									\
	f(TAB)										\
	f(BACKSPACE)								\
	f(INSERT)									\
	f(DELETE)									\
	f(RIGHT)									\
	f(LEFT)										\
	f(DOWN)										\
	f(UP)										\
	f(PAGE_UP)									\
	f(PAGE_DOWN)								\
	f(HOME)										\
	f(END)										\
	f(CAPS_LOCK)								\
	f(SCROLL_LOCK)								\
	f(NUM_LOCK)									\
	f(PRINT_SCREEN)								\
	f(PAUSE)									\
	f(F1)										\
	f(F2)										\
	f(F3)										\
	f(F4)										\
	f(F5)										\
	f(F6)										\
	f(F7)										\
	f(F8)										\
	f(F9)										\
	f(F10)										\
	f(F11)										\
	f(F12)										\
	f(F13)										\
	f(F14)										\
	f(F15)										\
	f(F16)										\
	f(F17)										\
	f(F18)										\
	f(F19)										\
	f(F20)										\
	f(F21)										\
	f(F22)										\
	f(F23)										\
	f(F24)										\
	f(F25)										\
	f(KP_0)										\
	f(KP_1)										\
	f(KP_2)										\
	f(KP_3)										\
	f(KP_4)										\
	f(KP_5)										\
	f(KP_6)										\
	f(KP_7)										\
	f(KP_8)										\
	f(KP_9)										\
	f(KP_DECIMAL)								\
	f(KP_DIVIDE)								\
	f(KP_MULTIPLY)								\
	f(KP_SUBTRACT)								\
	f(KP_ADD)									\
	f(KP_ENTER)									\
	f(KP_EQUAL)									\
	f(LEFT_SHIFT)								\
	f(LEFT_CONTROL)								\
	f(LEFT_ALT)									\
	f(LEFT_SUPER)								\
	f(RIGHT_SHIFT)								\
	f(RIGHT_CONTROL)							\
	f(RIGHT_ALT)								\
	f(RIGHT_SUPER)								\
	f(MENU)

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

	// Create map from key name to GLFW int
	unordered_map<string, int> glfw_map{ KEYS(_KEY_INSERT) };

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
			string key = line.substr(pch+1);

			// Perform mapping of event name to event enum
			auto eventIter = event_map.find(value);
			if(eventIter == event_map.end())
			{
				DEBUG_LOG("Keyboard configuration file " << filename <<
						  " has invalid event name: " << value << "\n");
				event = EGE_NONE;
			}
			else
			{
				event = (EGameEvent)eventIter->second;
			}

			// Perform mapping of key name to GLFW key
			int glfwKey;
			auto keyIter = glfw_map.find(key);
			if(keyIter == glfw_map.end())
			{
				DEBUG_LOG("Keyboard configuration file " << filename <<
						  " has invalid key: " << key << "\n");
				glfwKey = GLFW_KEY_UNKNOWN;
			}
			else
			{
				glfwKey = keyIter->second;
			}

			m_KeyMap.insert(std::make_pair(glfwKey,event));
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
