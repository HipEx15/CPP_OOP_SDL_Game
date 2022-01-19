//Using SDL and standard IO
#include "Game.h"
#include <SDL.h>
#include <iostream>


//Screen dimension constants
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

int main(int argc, char* args[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Game* game = new Game();
	game->init("We Go", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	
	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->render();
		game->update();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}	
		
	}

	game->clean();
	delete game;

	return 0;
}