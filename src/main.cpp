#include <iostream>
#include "base/Platform.h"
#include "system/Entity.h"
#include "game/TestComp.h"
#include "game/Game.h"

using namespace std;

int main()
{
	if(!ProgramInitialize())
	{
		// @TODO - Log this error!
		return -1;
	}

	Game game;
	GameAttributes attributes(1600, 1200, "Tetrad " + VERSION_STRING, false, 8);
	game.Initialize(attributes);
	game.Run();
	game.Shutdown();

	return 0;
}

