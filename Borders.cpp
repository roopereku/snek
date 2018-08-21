#include "Borders.h"
#include "Renderer.h"

Borders::Borders(Vector2 min, Vector2 max, WorldSpace& ws) : ws(ws)
{
	this->min = min;
	this->max = max;
}

void Borders::draw()
{
	// Create a white rectangle in between of min and max

	Render::setColor(255, 255, 255);

	Vector2 tMin = ws.toScreen(min);
	Vector2 tMax = ws.toScreen(max);

	Render::line(tMin[X], tMin[Y], tMax[X], tMin[Y]);
	Render::line(tMax[X], tMin[Y], tMax[X], tMax[Y]);
	Render::line(tMax[X], tMax[Y], tMin[X], tMax[Y]);
	Render::line(tMin[X], tMax[Y], tMin[X], tMin[Y]);
}

bool Borders::pointInside(Vector2 point)
{
	return point > min && point < max;
}

bool Borders::pointInsideX(float position)
{
	return position > min[X] && position < max[X];
}

bool Borders::pointInsideY(float position)
{
	return position > min[Y] && position < max[Y];
}

Vector2 Borders::getMinimum()
{
	return min;
}

Vector2 Borders::getMaximum()
{
	return max;
}
