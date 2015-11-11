#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include "game/Base.h"
#include "Entity.h"

using namespace std;

int main()
{
	#if defined( WIN32 ) && !defined( _DEBUG )
	FreeConsole();
	#endif

	Random::SetSeed(static_cast<unsigned long>(chrono::system_clock::now().time_since_epoch().count()));

	for(size_t i = 0; i < 20; ++i)
	{
		cout << EntityManager::CreateEntity() << "\n";
	}
	cout << endl;

	return 0;
}
