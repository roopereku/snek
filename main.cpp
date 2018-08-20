#include <SDL2/SDL.h>

#include "Window.h"
#include "Renderer.h"
#include "Game.h"

#include <time.h>

int main(int argc, char** argv)
{
	srand(time(0));

	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		bool isRunning;
		Window win("", isRunning, true, 4, 2);

		Game game(win);

		while(isRunning)
		{
			SDL_Event evnt;
			while(SDL_PollEvent(&evnt))
			{
				win.input(evnt, isRunning);
				game.input(evnt);
			}

			game.update();

			Render::setColor(0, 0, 0);
			Render::clearScreen();

			game.draw();

			Render::render();
		}

		SDL_Quit();
	}
	else
		SDL_Log("Unable to initialize : %s!", SDL_GetError());

	return 0;
}
