#include "Point.h"
#include "Renderer.h"

Point::Point(Vector2 center)
{
	pointCenter = center;
	staticSize = Vector2(0.05f, 0.05f);

	generatedAt = std::chrono::high_resolution_clock::now();

	pulseCounter = 0.0f;
}

void Point::draw(WorldSpace& ws)
{
	float radius = fabs(sin(pulseCounter) / 20);
	pulseCounter+=0.03f;

	Vector2 size(radius, radius);
	Vector2 position = pointCenter - (size / 2);

	Render::setColor(200, 0, 0);
	Render::rect( ws.rectToScreen(position, size) );

	size/=2;
	position[Y]-= (size[H] / 2);
	position[X]+= (size[W] / 2);

	Render::setColor(0, 255, 0);
	Render::rect( ws.rectToScreen(position, size) );
}

bool Point::intersectsPoint(Vector2 ip)
{
	Vector2 min = pointCenter - staticSize;
	Vector2 max = pointCenter + staticSize;

	return ip > min && ip < max;
}

int Point::pick()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - generatedAt;

	return elapsed.count();
}


