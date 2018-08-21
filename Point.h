#ifndef POINT_H
#define POINT_H

#include "WorldSpace.h"
#include "Vector2.h"

#include <chrono>

class Point
{
public:
	Point(Vector2 center);

	void draw(WorldSpace& ws);
	bool intersectsPoint(Vector2 ip);
	int pick();

private:
	Vector2 pointCenter;
	Vector2 staticSize;

	float pulseCounter;

	// When the point was generated
	std::chrono::time_point<std::chrono::high_resolution_clock> generatedAt;
};

#endif
