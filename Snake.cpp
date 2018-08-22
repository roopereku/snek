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

	score = 0;
	alive = true;

	immortalityTimer = 0.1f;
	immortalityTimerMax = 30.0f;

	// Randomize snake colors
	r = rand() % 255 + 100;
	g = rand() % 255 + 100;
	b = rand() % 255 + 100;

	// Set key data
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
	/*	If a key was pressed and if it exists in keys,
	 *	modify the pressed value.
	 */

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
	/*	If the snake isn't alive, check whether the snake still has parts.
	 *	If it does, remove some. Else signal the SnakeHandler that this snake is dead.
	 */

	if(!alive)
	{
		if(parts.size() > 0)
		{
			parts.pop_back();
			return true;
		}

		return false;
	}

	// If the snake is immortal, advance the timer

	if(isImmortal())
	{
		if((immortalityTimer+=0.1f) >= immortalityTimerMax)
			immortalityTimer = 0.0f;
	}

	// If the snake head hits some point, get the index of that point
	int intersectionIndex = point.intersectsPoint(parts[0]);

	// If the index is positive, pick the point and add to the score
	if(intersectionIndex != -1)
	{
		//SDL_Log("Adding length! %d -> %d", (int)parts.size(), (int)parts.size() + 10);
		//SDL_Log("Adding speed! %.2f -> %.2f", speed, speed + 0.1f);
		
		/*	Now pick the apple, and add to the score.
		 *	The less time is spent picking the point, the more will be added to the score
		 */

		unsigned long long ls = score;
		float sec = point.pickPoint(intersectionIndex);
		float ms = sec / 1000;

		if(ms > 0)
		{
			int ns = ( 100 / sec ) / speed;
			score+=ns;
			SDL_Log("Score! %lu -> %lu    %.2f", ls, score, sec);
		}


		// If there are no points present, generate one
		if(point.count() <= 0)
			point.generate(borders);

		addLength(10);

		speed+=0.1f;
	}

	//	If the snake head isn't inside the borders, move it to the other side

	if(!borders.pointInside(parts[0]))
	{
		if(!borders.pointInsideX(parts[0][X]))
			parts[0][X] = (parts[0][X] > 0.0f) ? borders.getMinimum()[X] : borders.getMaximum()[X];
		if(!borders.pointInsideY(parts[0][Y]))
			parts[0][Y] = (parts[0][Y] > 0.0f) ? borders.getMinimum()[Y] : borders.getMaximum()[Y];
	}
	
	//	Loop through all the valid keys.

	for(auto& it : keys)
	{
		if(it.second.pressed)
			rotation+=it.second.effect;
	}

	// Calculate the snake direction
	float rot = toRadian(rotation);
	Vector2 direction(cos(rot), sin(rot));

	// A lambda that check if a snake head intersects with a snake part
	const auto partIntersection = [&](Vector2 head, Vector2 part) -> bool
	{
		constexpr float hitBoxRadius = 0.004f;

		Vector2 min = part - Vector2(hitBoxRadius, hitBoxRadius);
		Vector2 max = part + Vector2(hitBoxRadius, hitBoxRadius);

		return head > min && head < max;
	};

	// Move the snake and handle collision

	for(int i = 0; i < speed; i++)
	{
		Vector2 last = parts[0];
		parts[0]+=(direction * 0.003f);

		// Loop through the parts and move them
		for(size_t pi = 1; pi < parts.size(); pi++)
		{
			// Loop through the snake heads to check whether they hit the current part
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
	const auto drawHitbox = [&](Vector2 part) -> void
	{
		constexpr float hitBoxRadius = 0.004f;

		Vector2 position = part - Vector2(hitBoxRadius, hitBoxRadius);
		Vector2 size = Vector2(hitBoxRadius * 2, hitBoxRadius * 2);

		Render::rect( ws.rectToScreen(position, size) );
	};

	// Temporary values to hold snake colors
	float nr = r,
		  ng = g,
		  nb = b;

	// How much will be decremented from the values above per loop
	float dr, dg, db;

	/*	Give values to dr, dg and db.
	 *	The calculations below will give a gradient effetc
	 */

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
