#ifndef BORDERS_H
#define BORDERS_H

#include "WorldSpace.h"
#include "Vector2.h"

class Borders
{
public:
	Borders(Vector2 min, Vector2 max, WorldSpace& ws);

	void draw();

	bool pointInside(Vector2 point);
	bool pointInsideX(float position);
	bool pointInsideY(float position);

	// Position getters
	Vector2& getMinimum();
	Vector2& getMaximum();

private:
	Vector2 min; // Minimum position of borders
	Vector2 max; // Maximum position of borders

	WorldSpace& ws;
};


#endif
