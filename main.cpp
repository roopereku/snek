#include <SDL2/SDL.h>

#include "Window.h"
#include "Renderer.h"
#include "Vector2.h"
#include "WorldSpace.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <chrono>
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

class Point
{
public:
	Point(Vector2 center)
	{
		pointCenter = center;
		staticSize = Vector2(0.05f, 0.05f);

		generatedAt = std::chrono::high_resolution_clock::now();

		pulseCounter = 0.0f;
	}

	void draw(WorldSpace& ws)
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

	bool intersectsPoint(Vector2 ip)
	{
		Vector2 min = pointCenter - staticSize;
		Vector2 max = pointCenter + staticSize;

		return ip > min && ip < max;
	}

	int pick()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = end - generatedAt;

		return elapsed.count();
	}

private:
	Vector2 pointCenter;
	Vector2 staticSize;

	float pulseCounter;

	std::chrono::time_point<std::chrono::high_resolution_clock> generatedAt;
};

class PointHandler
{
public:
	PointHandler(WorldSpace& ws) : ws(ws)
	{
		spawnTimer = 0.0f;
		spawnTimerMax = 20.0f;
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

		points.push_back( Point(Vector2(x, y)) );
	}

	size_t intersectsPoint(Vector2 ip)
	{
		for(size_t i = 0; i < points.size(); i++)
		{
			if(points[i].intersectsPoint(ip))
				return i;
		}

		return -1;
	}

	size_t count()
	{
		return points.size();
	}

	float pickPoint(size_t index)
	{
		float score = points[index].pick();
		points.erase(points.begin() + index);

		return score;
	}

	Point& operator[](size_t index)
	{
		return points[index];
	}

	void update(Borders& borders)
	{
		if((spawnTimer+=0.1f) >= spawnTimerMax)
		{
			generate(borders);
			spawnTimer = 0.0f;
		}
	}

	void draw()
	{
		for(size_t i = 0; i < points.size(); i++)
			points[i].draw(ws);
	}

private:
	std::vector <Point> points;

	float spawnTimer;
	float spawnTimerMax;

	WorldSpace& ws;
};

class Snake
{
public:
	Snake(int id, Vector2 origin, WorldSpace& ws, int keyLeft, int keyRight, float direction, float sensitivity) : ws(ws)
	{
		parts.push_back(origin);
		addLength(50);

		snakeID = id;

		rotation = direction;
		speed = 1.0f;

		score = 0.0f;
		alive = true;

		immortalityTimer = 0.1f;
		immortalityTimerMax = 30.0f;

		r = rand() % 255 + 100;
		g = rand() % 255 + 100;
		b = rand() % 255 + 100;

		keys[keyLeft].pressed = keys[keyRight].pressed = 0;

		keys[keyLeft].effect = -sensitivity;
		keys[keyRight].effect = sensitivity;
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

	bool update(Borders& borders, PointHandler& point, std::vector <Snake>& other)
	{
		if(!alive)
		{
			if(parts.size() > 0)
			{
				parts.pop_back();
				return true;
			}

			return false;
		}

		if(isImmortal())
		{
			if((immortalityTimer+=0.1f) >= immortalityTimerMax)
				immortalityTimer = 0.0f;
		}


		int intersectionIndex = point.intersectsPoint(parts[0]);

		if(intersectionIndex != -1)
		{
			SDL_Log("Adding length! %d -> %d", (int)parts.size(), (int)parts.size() + 10);
			SDL_Log("Adding speed! %.2f -> %.2f", speed, speed + 0.1f);

			/*	TODO
			 *	Do some calculation to add more to the score the less time is spent
			 */

			float ls = score;
			float ns = point.pickPoint(intersectionIndex); 

			score+=ns;
			SDL_Log("Score! %.2f -> %.2f", ls, score);

			if(point.count() <= 0)
				point.generate(borders);

			addLength(10);

			speed+=0.1f;
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

		const auto partIntersection = [&](Vector2 head, Vector2 part) -> bool
		{
			constexpr float hitBoxRadius = 0.004f;

			Vector2 min = part - Vector2(hitBoxRadius, hitBoxRadius);
			Vector2 max = part + Vector2(hitBoxRadius, hitBoxRadius);

			return head > min && head < max;
		};

		for(int i = 0; i < speed; i++)
		{
			Vector2 last = parts[0];
			parts[0]+=(direction * 0.005f);

			for(size_t pi = 1; pi < parts.size(); pi++)
			{
				for(size_t si = 0; si < other.size(); si++)
				{
					if(!other[si].isImmortal() && partIntersection(other[si].getHeadPosition(), parts[pi]))
						other[si].kill();
				}
				
				Vector2 last2 = parts[pi];
				parts[pi] = last;
				last = last2;
			}
		}

		return true;
	}

	Vector2 getHeadPosition()
	{
		return parts[0];
	}

	void kill()
	{
		//SDL_Log("Snake killed!");
		alive = false;
	}

	bool isImmortal()
	{
		return immortalityTimer > 0.0f;
	}

	void draw()
	{
		const auto drawSphere = [&](Vector2 center)
		{
			//#define r rand() % 255 + 1
			//Render::setColor(r, r, r);

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

			Render::rect( ws.rectToScreen(position, size) );
		};

		float nr = r,
			  ng = g,
			  nb = b;

		float dr, dg, db;

		if(isImmortal())
		{
			dr = (float)r / (parts.size() / 2);
			dg = (float)g / (parts.size() / 2);
			db = (float)b / (parts.size() / 2);
		}

		else
		{
			dr = (float)r / parts.size();
			dg = (float)g / parts.size();
			db = (float)b / parts.size();
		}


		for(size_t i = 1; i < parts.size(); i++)
		{
			nr-=dr;
			ng-=dg;
			nb-=db;

			Render::setColor((int)nr, (int)ng, (int)nb);

			drawHitbox(parts[i]);
		}
	}

private:
	std::vector <Vector2> parts;

	int snakeID;

	float rotation;
	float speed;

	float score;
	bool alive;

	float immortalityTimer;
	float immortalityTimerMax;

	unsigned char r;
	unsigned char g;
	unsigned char b;

	struct Key { bool pressed = false; float effect = 0.0f; };
	std::map <int, Key> keys;

	WorldSpace& ws;
};

class SnakeHandler
{
public:
	void add(Snake snake)
	{
		snakes.push_back( snake );
	}

	void input(SDL_Event evnt)
	{
		for(size_t i = 0; i < snakes.size(); i++)
			snakes[i].input(evnt);
	}

	void draw()
	{
		for(size_t i = 0; i < snakes.size(); i++)
			snakes[i].draw();
	}

	bool update(Borders& borders, PointHandler& point)
	{
		std::vector <bool> snakeStates(snakes.size());
			
		for(size_t i = 0; i < snakes.size(); i++)
		{
			snakeStates[i] = snakes[i].update(borders, point, snakes);
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

private:
	std::vector <Snake> snakes;
};

class Game
{
public:
	/*	TODO
	 *	Currently border has to be symmetrical or the snake won't switch the side correctly.
	 */
	Game(Window& win) : ws(win), borders(Vector2(-0.7f, -0.7f), Vector2(0.7f, 0.7f), ws), point(ws)
	{
		ws[X]-=0.1f;
		ws[Y]-=0.1f;

		point.generate(borders);

		/* TODO
		 * Angles such as 45, 135, 225, 315 will make the snake eat itself.
		 */
		snakes.add(Snake(2, Vector2(0.0f, 0.2f), ws, SDLK_n, SDLK_m, 90.0f, 2.5f));
		snakes.add(Snake(2, Vector2(0.6f, 0.2f), ws, 0, 0, 90.0f, 2.5f));

		gameRunning = true;
	}

	void input(SDL_Event evnt)
	{
		snakes.input(evnt);

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
			if(!snakes.update(borders, point))
			{
				SDL_Log("End");
				gameRunning = false;
			}

			point.update(borders);
		}
	}

	void draw()
	{
		ws.draw();
		snakes.draw();
		borders.draw();
		point.draw();
	}

private:
	WorldSpace ws;

	Borders borders;
	SnakeHandler snakes;
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
