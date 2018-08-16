#include <SDL2/SDL.h>

#include "Window.h"
#include "Renderer.h"
#include "Vector2.h"
#include "WorldSpace.h"

#include <math.h>
#include <time.h>
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

	bool pointInsideX(float position)
	{
		return position > min[X] && position < max[X];
	}

	bool pointInsideY(float position)
	{
		return position > min[Y] && position < max[Y];
	}

	Vector2 getMinimum()
	{
		return min;
	}

	Vector2 getMaximum()
	{
		return max;
	}

private:
	Vector2 min;
	Vector2 max;

	WorldSpace& ws;
};

class PointHandler
{
public:
	PointHandler(WorldSpace& ws) : ws(ws)
	{
		pulseCounter = 0.0f;
		staticSize = Vector2(0.05f, 0.05f);
	}

	void generate(Borders& borders)
	{
		/*	TODO
		 *	Do some clever to calculate a position between min and max
		 */

		int xSize = (borders.getMaximum()[X] - borders.getMinimum()[X]) * 10;
		int ySize = (borders.getMaximum()[Y] - borders.getMinimum()[Y]) * 10;

		std::vector <float> xData(xSize);
		std::vector <float> yData(ySize);

		for(int xi = 0; xi < xSize; xi++)
			xData[xi] = borders.getMinimum()[X] + 0.1f * xi;

		for(int yi = 0; yi < ySize; yi++)
			yData[yi] = borders.getMinimum()[Y] + 0.1f * yi;

		SDL_Log("%d : %d", xSize, ySize);

		float x = xData[ (rand() % xSize + 2) - 1 ];
		float y = yData[ (rand() % ySize + 2) - 1 ];

		SDL_Log("%.2f : %.2f", x, y);

		pointCenter = Vector2(x, y);
		pulseCounter = 0.0f;
	}

	bool intersectsPoint(Vector2 ip)
	{
		Vector2 min = pointCenter - staticSize;
		Vector2 max = pointCenter + staticSize;

		return ip > min && ip < max;
	}

	void draw()
	{
		float radius = fabs(sin(pulseCounter) / 20);
		pulseCounter+=0.03f;

		Vector2 size(radius, radius);
		Vector2 position = pointCenter - (size / 2);

		Render::setColor(200, 0, 0);
		Render::rect( ws.rectToScreen(position, size) );

		size/=2;
		position[Y]-= (size[H] / 2);
		position[X]+= (size[W] / 2);

		Render::setColor(0, 255, 0);
		Render::rect( ws.rectToScreen(position, size) );
	}

private:
	Vector2 pointCenter;
	Vector2 staticSize;

	float pulseCounter;
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

	bool update(Borders& borders, PointHandler& point)
	{
		if(point.intersectsPoint(parts[0]))
		{
			SDL_Log("Adding length! %d -> %d", (int)parts.size(), (int)parts.size() + 10);

			point.generate(borders);
			addLength(10);
		}

		if(!borders.pointInside(parts[0]))
		{
			if(!borders.pointInsideX(parts[0][X]))
				parts[0][X] = -parts[0][X];
			if(!borders.pointInsideY(parts[0][Y]))
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
				return false;
			
			Vector2 last2 = parts[i];
			parts[i] = last;
			last = last2;
		}

		return true;
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

			Vector2 position = part - Vector2(hitBoxRadius, hitBoxRadius);
			Vector2 size = Vector2(hitBoxRadius * 2, hitBoxRadius * 2);

			Render::setColor(0, 100, 0);
			Render::rect( ws.rectToScreen(position, size) );
		};

		for(size_t i = 1; i < parts.size(); i++)
		{
			Render::setColor(0, 255, 0);

			/*Vector2 tPos = ws.toScreen(parts[i]);
			Vector2 tPosLast = ws.toScreen(parts[i - 1]);
			*/
		//	Render::line(tPos[X], tPos[Y], tPosLast[X], tPosLast[Y]);
			//Render::dot(tPos[X], tPos[Y]);

		//	drawSphere(parts[i]);
			drawHitbox(parts[i]);
		}
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
	Game(Window& win) : ws(win), snake(Vector2(0.0f, 0.0f), ws), borders(Vector2(-0.7f, -0.7f), Vector2(0.7f, 0.7f), ws), point(ws)
	{
		ws[X]-=0.1f;
		ws[Y]-=0.1f;

		snake.addLength(50);
		point.generate(borders);

		gameRunning = true;
	}

	void input(SDL_Event evnt)
	{
		if(gameRunning)
			snake.input(evnt);

		if(evnt.type == SDL_KEYDOWN)
		{
			switch(evnt.key.keysym.sym)
			{
				case SDLK_w: ws[Y]+=0.1f; break;
				case SDLK_s: ws[Y]-=0.1f; break;

				case SDLK_a: ws[X]-=0.1f; break;
				case SDLK_d: ws[X]+=0.1f; break;
			}
		}
	}

	void update()
	{
		if(gameRunning)
		{
			if(!snake.update(borders, point))
				gameRunning = false;
		}
	}

	void draw()
	{
		ws.draw();
		snake.draw();
		borders.draw();
		point.draw();
	}

private:
	WorldSpace ws;

	Snake snake;
	Borders borders;
	PointHandler point;

	bool gameRunning;
};

int main()
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
