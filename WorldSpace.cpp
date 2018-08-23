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

	// How much will be incremented to x and y per loop
	float inc = 0.1f;

	// There are here to make things easier later
	Vector2 start = toScreen(Vector2(-radiusX, -radiusY));
	Vector2 end = toScreen(Vector2(radiusX, radiusY));

	// Draw vertical lines
	for(float x = -radiusX + inc; x < radiusX; x+=inc)
	{
		Vector2 current = toScreen(Vector2(x, 0.0f));
		Render::line(current[X], start[Y], current[X], end[Y]);
	}	

	// Draw horizontal lines
	for(float y = -radiusY + inc; y < radiusY; y+=inc)
	{
		Vector2 current = toScreen(Vector2(0.0f, y));
		Render::line(start[X], current[Y], end[X], current[Y]);
	}	
}

float& WorldSpace::operator[](int index)
{
	// Since this function returns a reference, the game could crash on an invalid argument

	switch(index)
	{
		case X: return radiusX;
		case Y: return radiusY;
	}
}

Vector2 WorldSpace::toScreen(Vector2 vec)
{
	// Translate vec to screen.
	return Vector2( ((vec[X] + radiusX) / 2) * (win['W'] / radiusX), ((vec[Y] + radiusY) / 2) * (win['H'] / radiusY) );
}

SDL_Rect WorldSpace::rectToScreen(Vector2 position, Vector2 size)
{
	// Translate a rectangle to the screen

	Vector2 worldRadius(radiusX, radiusY);
	size-=(worldRadius);

	position = toScreen(position);
	size = toScreen(size);

	return { (int)position[X], (int)position[Y], (int)size[W], (int)size[H] };
}
