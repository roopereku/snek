#include "SnakeHandler.h"

SnakeHandler::SnakeHandler(WorldSpace& ws)
{
	add(Snake(2, Vector2(0.0f, 0.2f), ws, SDLK_n, SDLK_m, 45.0f, 2.5f));
	//add(Snake(2, Vector2(0.6f, 0.2f), ws, 0, 0, 90.0f, 2.5f));

	initialSnakeAmount = snakes.size();
}

void SnakeHandler::add(Snake snake)
{
	snakes.push_back( snake );
}

void SnakeHandler::input(SDL_Event evnt)
{
	for(size_t i = 0; i < snakes.size(); i++)
		snakes[i].input(evnt);
}

void SnakeHandler::draw()
{
	for(size_t i = 0; i < snakes.size(); i++)
		snakes[i].draw();
}

bool SnakeHandler::update(Borders& borders, PointHandler& point)
{
	std::vector <bool> snakeStates(snakes.size());
		
	for(size_t i = 0; i < snakes.size(); i++)
		snakeStates[i] = snakes[i].update(borders, point, snakes);

	if(initialSnakeAmount == 1)
	{
		return snakeStates[0];
	}

	std::vector <size_t> successes;
	int successfulReturns = 0;

	for(size_t i = 0; i < snakeStates.size(); i++)
	{
		if(snakeStates[i] == true)
		{
			successfulReturns++;
			successes.push_back(i);
		}
	}

	if(successfulReturns <= 1)
	{
		SDL_Log("Player %d wins!", (int)successes[0] + 1);
		return false;
	}

	return true;
}

int SnakeHandler::initialSnakes()
{
	return initialSnakeAmount;
}


