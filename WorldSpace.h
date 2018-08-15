#ifndef WORLD_SPACE_H
#define WORLD_SPACE_H

#include "Window.h"
#include "Vector2.h"

class WorldSpace
{
public:
	WorldSpace(Window& win);

	void draw();

	float& operator[](int index);

	Vector2 toScreen(Vector2 vec);

private:
	float radiusX;
	float radiusY;

	Window& win;
};

#endif
