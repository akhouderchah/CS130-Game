#include <SFML/Graphics.hpp>
#include <iostream>
#include "system/Entity.h"
#include "game/TestComp.h"
#include "game/Game.h"

using namespace std;

int main()
{
	#if defined( WIN32 ) && !defined( _DEBUG )
	FreeConsole();
	#endif

	Game game;
	GameAttributes attributes(1600, 1200);
	attributes.m_ContextSettings.antialiasingLevel = 8;
	attributes.m_WindowTitle = "Tetrad " + VERSION_STRING;
	game.Initialize(attributes);
	game.Run();
	game.Shutdown();

	/*
	for(size_t i = 0; i < 20; ++i)
	{
		Entity temp(EntityManager::CreateEntity());
		temp.AddComponent(TestComponent());
	}

	TestComponent test = *Entity(1).GetAs<TestComponent>();
	test.Tick(0.0);

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

	return 0;
}

