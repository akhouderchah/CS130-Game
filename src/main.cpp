#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include "base/Base.h"
#include "system/EntityManager.h"
#include "system/Entity.h"
#include "game/TestComp.h"
#include "game/Game.h"

using namespace std;

int main()
{
	#if defined( WIN32 ) && !defined( _DEBUG )
	FreeConsole();
	#endif

	Random::SetSeed(static_cast<unsigned long>(chrono::system_clock::now().time_since_epoch().count()));

	EntityManager::Initialize();
	Game game;
	game.Initialize(GameAttributes());
	game.Run();
	game.Shutdown();

	for(size_t i = 0; i < 20; ++i)
	{
		Entity temp(EntityManager::CreateEntity());
		temp.AddComponent(TestComponent());
	}

	TestComponent test = *Entity(1).GetAs<TestComponent>();
	test.Tick(0.0);

	/*
	for(size_t i = 5; i < 12; ++i)
	{
		Entity temp(i);
		EntityManager::DestroyEntity(temp);
	}

	for(size_t i = 0; i < 10; ++i)
	{
		cout << EntityManager::CreateEntity() << "\n";
	}
	*/

	cout << endl;

	EntityManager::Shutdown();

	return 0;
}

