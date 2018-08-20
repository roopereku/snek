#ifndef POINT_HANDLER_H
#define POINT_HANDLER_H

#include "WorldSpace.h"
#include "Borders.h"
#include "Point.h"

#include <vector>

class PointHandler
{
public:
	PointHandler(WorldSpace& ws);

	void generate(Borders& borders);
	
	size_t intersectsPoint(Vector2 ip);
	size_t count();

	float pickPoint(size_t index);

	Point& operator[](size_t index);
	void update(Borders& borders);
	void draw();

private:
	std::vector <Point> points;

	float spawnTimer;
	float spawnTimerMax;
	int maxPoints;

	WorldSpace& ws;
};


#endif
