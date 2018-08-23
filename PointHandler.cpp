#include "PointHandler.h"

PointHandler::PointHandler(WorldSpace& ws, Config& config) : ws(ws)
{
	spawnTimer = 0.0f;

	// Set things relative to configuration data
	spawnTimerMax = config.fromSingle("-appledelay");
	maxPoints = config.fromSingle("-maxapples");
}

void PointHandler::generate(Borders& borders)
{
	/*	TODO
	 *	Do something clever to calculate a position between min and max
	 */

	// Amount of possible positions
	int xSize = (borders.getMaximum()[X] - borders.getMinimum()[X]) * 10;
	int ySize = (borders.getMaximum()[Y] - borders.getMinimum()[Y]) * 10;

	// Vector to store possible positions in
	std::vector <float> xData(xSize);
	std::vector <float> yData(ySize);

	// Fill the vectors

	for(int xi = 0; xi < xSize; xi++)
		xData[xi] = borders.getMinimum()[X] + 0.1f * xi;

	for(int yi = 0; yi < ySize; yi++)
		yData[yi] = borders.getMinimum()[Y] + 0.1f * yi;

	// Get a random position and create a point

	float x = xData[ (rand() % xSize + 2) - 1 ];
	float y = yData[ (rand() % ySize + 2) - 1 ];

	points.push_back( Point(Vector2(x, y)) );
}

size_t PointHandler::intersectsPoint(Vector2 ip)
{
	/*	If ip intersects some point,
	 *	return the index of that point
	 */

	for(size_t i = 0; i < points.size(); i++)
	{
		if(points[i].intersectsPoint(ip))
			return i;
	}

	return -1;
}

size_t PointHandler::count()
{
	return points.size();
}

float PointHandler::pickPoint(size_t index)
{
	// Delete point at index x and return the time it took to collect it

	float score = points[index].pick();
	points.erase(points.begin() + index);

	return score;
}

Point& PointHandler::operator[](size_t index)
{
	return points[index];
}

void PointHandler::update(Borders& borders)
{
	/*	If spawnTimer is greater than spawnTimerMax,
	 *	set spawnTimer to 0.0 and generate a new point
	 *	if the amount of points + 1 is less than maxPoints
	 */

	if((spawnTimer+=0.1f) >= spawnTimerMax)
	{
		if(points.size() + 1 <= (size_t)maxPoints)
			generate(borders);
		spawnTimer = 0.0f;
	}

	for(size_t i = 0; i < points.size(); i++)
		points[i].update();
}

void PointHandler::draw()
{
	for(size_t i = 0; i < points.size(); i++)
		points[i].draw(ws);
}
