#include <SDL2/SDL.h>

#include "Window.h"
#include "Renderer.h"
#include "Vector2.h"
#include "WorldSpace.h"

#include <math.h>
#include <stdlib.h>
#include <vector>
#include <map>

float toRadian(float degrees)
{
	return degrees * 0.0174532925;
}

class Borders
{
public:
	Borders(Vector2 min, Vector2 max, WorldSpace& ws) : ws(ws)
	{
		this->min = min;
		this->max = max;
	}

	void draw()
	{
		Render::setColor(255, 255, 255);

		Vector2 tMin = ws.toScreen(min);
		Vector2 tMax = ws.toScreen(max);

		Render::line(tMin[X], tMin[Y], tMax[X], tMin[Y]);
		Render::line(tMax[X], tMin[Y], tMax[X], tMax[Y]);
		Render::line(tMax[X], tMax[Y], tMin[X], tMax[Y]);
		Render::line(tMin[X], tMax[Y], tMin[X], tMin[Y]);
	}

	bool pointInside(Vector2 point)
	{
		return point > min && point < max;
	}

private:
	Vector2 min;
	Vector2 max;

	WorldSpace& ws;
};

class Snake
{
public:
	Snake(Vector2 origin, WorldSpace& ws) : ws(ws)
	{
		parts.push_back(origin);

		rotation = 0.0f;
		speed = 0.005f;

		keys[SDLK_LEFT].pressed = keys[SDLK_RIGHT].pressed = 0;

		keys[SDLK_LEFT].effect = -2.5f;
		keys[SDLK_RIGHT].effect = 2.5f;
	}

	void addLength(int amount)
	{
		for(int i = 0; i < amount; i++)
		{
			parts.push_back(parts[parts.size() - 1]);
		}
	}

	void input(SDL_Event evnt)
	{
		int keyID = -1;
		bool pressed = false;

		switch(evnt.type)
		{
			case SDL_KEYDOWN: pressed = true; break;
			case SDL_KEYUP: pressed = false; break;

			default: return;
		}

		keyID = evnt.key.keysym.sym;

		if(keys.find(keyID) != keys.end())
			keys[keyID].pressed = pressed;
	}

	void update(Borders& borders)
	{
		/*	TODO
		 *	Add a check to see which angle the snake is in, and adjust
		 *	the position where the snake appears when going out of bounds.
		 */

		if(!borders.pointInside(parts[0]))
		{
			parts[0][X] = -parts[0][X];
			parts[0][Y] = -parts[0][Y];
		}

		for(auto& it : keys)
		{
			if(it.second.pressed)
				rotation+=it.second.effect;
		}

		float rot = toRadian(rotation);
		Vector2 direction(cos(rot), sin(rot));

		Vector2 last = parts[0];
		parts[0]+=(direction * speed);

		const auto partIntersection = [&](Vector2 part) -> bool
		{
			constexpr float hitBoxRadius = 0.004f;

			Vector2 min = part - Vector2(hitBoxRadius, hitBoxRadius);
			Vector2 max = part + Vector2(hitBoxRadius, hitBoxRadius);

			return parts[0] > min && parts[0] < max;
		};

		for(size_t i = 1; i < parts.size(); i++)
		{
			if(partIntersection(parts[i]))
			{
				SDL_Log("Hit %d!", (int)i);
			}
			
			Vector2 last2 = parts[i];
			parts[i] = last;
			last = last2;
		}
	}

	void draw()
	{
		const auto drawSphere = [&](Vector2 center)
		{
			#define r rand() % 255 + 1
			Render::setColor(r, r, r);

			for(int i = 0; i < 360; i++)
			{
				float rad = toRadian(i);
				Vector2 current = ws.toScreen(center + (Vector2(cos(rad), sin(rad)) * 0.05f));

				Render::dot(current[X], current[Y]);
			}
		};

		const auto drawHitbox = [&](Vector2 part) -> void
		{
			constexpr float hitBoxRadius = 0.004f;

			Vector2 min = ws.toScreen(part - Vector2(hitBoxRadius, hitBoxRadius));
			Vector2 max = ws.toScreen(part + Vector2(hitBoxRadius, hitBoxRadius));
			
			Render::setColor(255, 0, 0);
			Render::line(min[X], min[Y], max[X], min[Y]);
			Render::line(max[X], min[Y], max[X], max[Y]);
			Render::line(max[X], max[Y], min[X], max[Y]);
			Render::line(min[X], max[Y], min[X], min[Y]);
		};

		for(size_t i = 1; i < parts.size(); i++)
		{
			Render::setColor(0, 255, 0);

			Vector2 tPos = ws.toScreen(parts[i]);
			Vector2 tPosLast = ws.toScreen(parts[i - 1]);

			Render::line(tPos[X], tPos[Y], tPosLast[X], tPosLast[Y]);

		//	drawSphere(parts[i]);
		//	drawHitbox(parts[i]);
		}

		float rot = toRadian(rotation);
		Vector2 direction = ws.toScreen(parts[0] + (Vector2(cos(rot), sin(rot)) * 0.1f));
		
		Render::setColor(255, 0, 0);
		Render::dot(direction[X], direction[Y]);
	}

private:
	std::vector <Vector2> parts;

	float rotation;
	float speed;

	struct Key { bool pressed = false; float effect = 0.0f; };
	std::map <int, Key> keys;

	WorldSpace& ws;
};

class Game
{
public:
	Game(Window& win) : ws(win), snake(Vector2(0.0f, 0.0f), ws), borders(Vector2(-0.5f, -0.5f), Vector2(0.5f, 0.5f), ws)
	{
		snake.addLength(50);
	}

	void input(SDL_Event evnt)
	{
		snake.input(evnt);
	}

	void update()
	{
		snake.update(borders);
	}

	void draw()
	{
		ws.draw();
		snake.draw();
		borders.draw();
	}

private:
	WorldSpace ws;

	Snake snake;
	Borders borders;
};

int main()
{
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
