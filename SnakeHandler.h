#ifndef SNAKE_HANDLER_H
#define SNAKE_HANDLER_H

#include "WorldSpace.h"
#include "PointHandler.h"
#include "Borders.h"
#include "Config.h"
#include "Snake.h"

#include <SDL2/SDL.h>

#include <vector>

class SnakeHandler
{
public:
	SnakeHandler(WorldSpace& ws, Config& config);

	void add(Snake snake);
	void input(SDL_Event evnt);
	void draw();

	bool update(Borders& borders, PointHandler& point);
	int initialSnakes();

private:
	std::vector <Snake> snakes;

	// Used to check whether the game was started in singleplayer
	int initialSnakeAmount;
};

#endif
