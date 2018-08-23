#include "Point.h"
#include "Renderer.h"

Point::Point(Vector2 center)
{
	pointCenter = center;
	staticSize = Vector2(0.05f, 0.05f);

	// Get the current time
	generatedAt = std::chrono::high_resolution_clock::now();

	pulseCounter = 0.0f;
	visibility = 255;
}

void Point::draw(WorldSpace& ws)
{
	// How large the apple will be
	float radius = fabs(sin(pulseCounter) / 20);

	// Calculate apple min and max
	Vector2 size(radius, radius);
	Vector2 position = pointCenter - (size / 2);

	// Draw the apple
	Render::setColor(visibility, 0, 0);
	Render::rect( ws.rectToScreen(position, size) );

	// Shrink min and max for the leaf
	size/=2;
	position[Y]-= (size[H] / 2);
	position[X]+= (size[W] / 2);

	// Draw the leaf
	Render::setColor(0, visibility, 0);
	Render::rect( ws.rectToScreen(position, size) );
}

void Point::update()
{
	pulseCounter+=0.03f;

	/*	Get elapsed time of apple.
	 *	The longer it has been present, the darker it will get.
	 */
	std::chrono::duration <float> elapsed = std::chrono::high_resolution_clock::now() - generatedAt;

	// Check whether visibility is greater than 50. If it is, make the apple darker
	if(visibility > 50)
		visibility = 255 - elapsed.count() * 15;
}

bool Point::intersectsPoint(Vector2 ip)
{
	Vector2 min = pointCenter - staticSize;
	Vector2 max = pointCenter + staticSize;

	return ip > min && ip < max;
}

int Point::pick()
{
	// Get the elapsed time
	std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - generatedAt;

	// Return seconds
	return elapsed.count();
}


