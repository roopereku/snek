#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "WorldSpace.h"
#include "Borders.h"
#include "SnakeHandler.h"
#include "PointHandler.h"
#include "Config.h"
#include "Vector2.h"

#include <SDL2/SDL.h>

class Game
{
public:
	Game(Window& win, Config& config);

	void input(SDL_Event evnt);
	void update();
	void draw();

private:
	WorldSpace ws;

	Borders borders;
	SnakeHandler snakes;
	PointHandler point;

	bool gameRunning;
};

#endif
