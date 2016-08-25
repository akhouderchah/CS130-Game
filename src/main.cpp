#include <iostream>
#include "Platform.h"
#include "EntityManager.h"
#include "Game.h"

using namespace std;

int main()
{
	if(!programInitialize())
	{
		// @TODO - Log this error!
		return -1;
	}

	Game game;
	GameAttributes attributes(1600, 1200, "Tetrad " + VERSION_STRING, false, 8);
	if(!game.Initialize(attributes))
	{
		return -1;
	}
	
	game.Run();
	game.Shutdown();

	return 0;
}
