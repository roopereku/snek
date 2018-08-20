#include "PointHandler.h"

PointHandler::PointHandler(WorldSpace& ws) : ws(ws)
{
	spawnTimer = 0.0f;
	spawnTimerMax = 20.0f;
}

void PointHandler::generate(Borders& borders)
{
	/*	TODO
	 *	Do some clever to calculate a position between min and max
	 */

	int xSize = (borders.getMaximum()[X] - borders.getMinimum()[X]) * 10;
	int ySize = (borders.getMaximum()[Y] - borders.getMinimum()[Y]) * 10;

	std::vector <float> xData(xSize);
	std::vector <float> yData(ySize);

	for(int xi = 0; xi < xSize; xi++)
		xData[xi] = borders.getMinimum()[X] + 0.1f * xi;

	for(int yi = 0; yi < ySize; yi++)
		yData[yi] = borders.getMinimum()[Y] + 0.1f * yi;

	SDL_Log("%d : %d", xSize, ySize);

	float x = xData[ (rand() % xSize + 2) - 1 ];
	float y = yData[ (rand() % ySize + 2) - 1 ];

	SDL_Log("%.2f : %.2f", x, y);

	points.push_back( Point(Vector2(x, y)) );
}

size_t PointHandler::intersectsPoint(Vector2 ip)
{
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
	if((spawnTimer+=0.1f) >= spawnTimerMax)
	{
		generate(borders);
		spawnTimer = 0.0f;
	}
}

void PointHandler::draw()
{
	for(size_t i = 0; i < points.size(); i++)
		points[i].draw(ws);
}
