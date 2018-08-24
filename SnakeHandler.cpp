#include "SnakeHandler.h"

SnakeHandler::SnakeHandler(WorldSpace& ws, Config& config)
{
	/*	Some presets for snake data.
	 *	These will likely be set in the
	 *	default config file
	 */

	/*	TODO
	 *	Think of some clever way to define players keys
	 *	in the config file
	 */

	constexpr int keys[][2]
	{
		{ SDLK_LEFT, SDLK_RIGHT },
		{ SDLK_n, SDLK_m },
		{ SDLK_z, SDLK_x },
		{ SDLK_j, SDLK_k }
	};

	const Vector2 origin[]
	{
		Vector2(-0.2f, -0.2f),
		Vector2(-0.2f, +0.2f),
		Vector2(+0.2f, +0.2f),
		Vector2(+0.2f, -0.2f)
	};

	// Create the players

	const int maxPlayers = config.fromSingle("-maxplayers");
	int numPlayers = config.fromSingle("-players");

	if(numPlayers > maxPlayers)
	{
		SDL_Log("There can be only %d players!", maxPlayers);
		numPlayers = maxPlayers;
	}

	SDL_Log("Creating %d players!", numPlayers);

	for(int i = 0; i < numPlayers; i++)
	{
		/*	Check if i is less than the size of keys/origin.
		 *	This is here to prevent a crash.
		 */

		int keyL = i < 4 ? keys[i][0] : 0;
		int keyR = i < 4 ? keys[i][1] : 0;

		Vector2 position = i < 4 ? origin[i] : Vector2(0.0f, 0.0f);

		add( Snake(i, position, ws, keyL, keyR, config) );
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
	/*	Create a vector that contains the return values
	 *	of each snake update calls.
	 */

	std::vector <bool> snakeStates(snakes.size());
		
	for(size_t i = 0; i < snakes.size(); i++)
		snakeStates[i] = snakes[i].update(borders, point, snakes);

	// If there was only 1 snake to begin with, return the first element

	if(initialSnakeAmount == 1)
	{
		return snakeStates[0];
	}

	/*	Create a vector that contains the successful returns.
	 *	This will be used to determine the winning player.
	 */

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

	// If there's only 1 snake left, make the game end and print the winner

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
