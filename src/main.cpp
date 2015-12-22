#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include "game/Base.h"
#include "game/EntityManager.h"
#include "content/TestComp.h"

using namespace std;

int main()
{
	#if defined( WIN32 ) && !defined( _DEBUG )
	FreeConsole();
	#endif

	Random::SetSeed(static_cast<unsigned long>(chrono::system_clock::now().time_since_epoch().count()));

	EntityManager::Initialize();

	for(size_t i = 0; i < 20; ++i)
	{
		Entity temp(EntityManager::CreateEntity());
		EntityManager::AddComponent(temp, TestComponent());
	}

	TestComponent test = *EntityManager::GetComponent<TestComponent>(1);
	test.Tick();

	for(size_t i = 5; i < 12; ++i)
	{
		Entity temp(i);
		EntityManager::DestroyEntity(temp);
	}

	for(size_t i = 0; i < 10; ++i)
	{
		cout << EntityManager::CreateEntity() << "\n";
	}

	cout << endl;

	return 0;
}
