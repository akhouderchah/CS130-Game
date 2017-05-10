#include "Platform.h"
#include "EntityManager.h"
#include "TetradGame.h"

#include <iostream>

using namespace std;

int main()
{
	if(!programInitialize())
	{
		ERROR("Failed to do platform-specific program initialization!", EEB_CONTINUE);
		return -1;
	}

	TetradGame game;
	GameAttributes attributes(1280, 960, "Tetrad " + VERSION_STRING, false, 4);
	if(!game.Initialize(attributes))
	{
		return -1;
	}

	game.Run();
	game.Shutdown();

	return 0;
}
