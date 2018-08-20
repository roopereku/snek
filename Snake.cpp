#include "Snake.h"
#include "Renderer.h"

float toRadian(float degrees)
{
	return degrees * 0.0174532925;
}

Snake::Snake(int id, Vector2 origin, WorldSpace& ws, int keyLeft, int keyRight, float direction, float sensitivity) : ws(ws)
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

void Snake::addLength(int amount)
{
	for(int i = 0; i < amount; i++)
	{
		parts.push_back(parts[parts.size() - 1]);
	}
}

void Snake::input(SDL_Event evnt)
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

bool Snake::update(Borders& borders, PointHandler& point, std::vector <Snake>& other)
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

		float ls = score;
		float sec = point.pickPoint(intersectionIndex);
		float ns = 100 / sec;

		score+=ns;
		SDL_Log("Score! %.2f -> %.2f    %.2f", ls, score, sec);

		if(point.count() <= 0)
			point.generate(borders);

		addLength(10);

		speed+=0.1f;
	}

	if(!borders.pointInside(parts[0]))
	{
		if(!borders.pointInsideX(parts[0][X]))
			parts[0][X] = (parts[0][X] > 0.0f) ? borders.getMinimum()[X] : borders.getMaximum()[X];
		if(!borders.pointInsideY(parts[0][Y]))
			parts[0][Y] = (parts[0][Y] > 0.0f) ? borders.getMinimum()[Y] : borders.getMaximum()[Y];
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

Vector2 Snake::getHeadPosition()
{
	return parts[0];
}

void Snake::kill()
{
	//SDL_Log("Snake killed!");
	alive = false;
}

bool Snake::isImmortal()
{
	return immortalityTimer > 0.0f;
}

void Snake::draw()
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
		dr = (float)r / (parts.size() * 1.5f);
		dg = (float)g / (parts.size() * 1.5f);
		db = (float)b / (parts.size() * 1.5f);
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
