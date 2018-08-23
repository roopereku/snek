#include "Game.h"
#include "Renderer.h"

Game::Game(Window& win, Config& config) : ws(win), borders(Vector2(-0.7f, -0.7f), Vector2(0.7f, 0.7f), ws), snakes(ws, config), point(ws, config)
{
	ws[X]-=0.1f;
	ws[Y]-=0.1f;

	point.generate(borders);

	gameRunning = true;
	gamePaused = true;
}

void Game::input(SDL_Event evnt)
{
	snakes.input(evnt);

	if(evnt.type == SDL_KEYDOWN)
	{
		switch(evnt.key.keysym.sym)
		{
			// Camera controls
			case SDLK_w: ws[Y]-=0.1f; break;
			case SDLK_s: ws[Y]+=0.1f; break;

			case SDLK_a: ws[X]+=0.1f; break;
			case SDLK_d: ws[X]-=0.1f; break;

			case SDLK_RETURN:
				// Swap the state of gamePaused
				gamePaused = gamePaused ? false : true;
			break;
		}
	}
}

void Game::update()
{
	// Update if game is running and isn't paused
	if(gameRunning && !gamePaused)
	{
		/*	If updating snakes returns false, there's only 0 - 1 snakes left.
		 *	GameRunning will be set to false if this happens
		 */

		if(!snakes.update(borders, point))
		{
			SDL_Log("End");
			gameRunning = false;
		}

		point.update(borders);
	}
}

void Game::draw()
{
	ws.draw();
	borders.draw();

	snakes.draw();
	point.draw();
}
