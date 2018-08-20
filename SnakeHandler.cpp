#include "SnakeHandler.h"

SnakeHandler::SnakeHandler(WorldSpace& ws, Config& config)
{
	constexpr int keys[2][2]
	{
		{ SDLK_LEFT, SDLK_RIGHT },
		{ SDLK_n, SDLK_m }
	};

	constexpr float direction[2]
	{
		0.0f,
		180.0f
	};

	const Vector2 origin[2]
	{
		Vector2(-0.2f, -0.2f),
		Vector2(+0.2f, +0.2f)
	};

	int numPlayers = config.fromSingle("-players");
	SDL_Log("Creating %d players!", numPlayers);

	for(int i = 0; i < numPlayers; i++)
	{
		SDL_Log("[Snake %d] Sensitivity : %.2f", i + 1, config.fromMulti("-sensitivity", i));;
		add( Snake(i + 1, origin[i], ws, keys[i][0], keys[i][1], direction[i], config.fromMulti("-sensitivity", i)) );
	}

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


