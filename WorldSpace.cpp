#include "WorldSpace.h"
#include "Renderer.h"

WorldSpace::WorldSpace(Window& win) : win(win)
{
	radiusX = 1.0f;
	radiusY = 1.0f;
}

void WorldSpace::draw()
{
	Render::setColor(25, 25, 25);

	float inc = 0.1f;

	Vector2 start = toScreen(Vector2(-radiusX, -radiusY));
	Vector2 end = toScreen(Vector2(radiusX, radiusY));

	for(float x = -radiusX + inc; x < radiusX; x+=inc)
	{
		Vector2 current = toScreen(Vector2(x, 0.0f));
		Render::line(current[X], start[Y], current[X], end[Y]);
	}	
	for(float y = -radiusY + inc; y < radiusY; y+=inc)
	{
		Vector2 current = toScreen(Vector2(0.0f, y));
		Render::line(start[X], current[Y], end[X], current[Y]);
	}	
}

float& WorldSpace::operator[](int index)
{
	switch(index)
	{
		case X: return radiusX;
		case Y: return radiusY;
	}
}

Vector2 WorldSpace::toScreen(Vector2 vec)
{
	return Vector2( ((vec[X] + radiusX) / 2) * (win['W'] / radiusX), ((vec[Y] + radiusY) / 2) * (win['H'] / radiusY) );
}
