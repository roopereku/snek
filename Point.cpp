#include "Point.h"
#include "Renderer.h"
Point::Point(Vector2 center){pointCenter=center;staticSize=Vector2(0.05f,0.05f);generatedAt=std::chrono::high_resolution_clock::now();pulseCounter=0.0f;visibility=255;}void Point::draw(WorldSpace&ws){float radius=fabs(sin(pulseCounter)/20);Vector2 size(radius,radius);Vector2 position=pointCenter-(size/2);Render::setColor(visibility,0,0);Render::rect(ws.rectToScreen(position,size));size/=2;position[Y]-=(size[H]/2);position[X]+=(size[W]/2);Render::setColor(0,visibility,0);Render::rect(ws.rectToScreen(position,size));}void Point::update(){pulseCounter+=0.03f;std::chrono::duration<float>elapsed=std::chrono::high_resolution_clock::now()-generatedAt;if(visibility>50)visibility=255-elapsed.count()*15;}bool Point::intersectsPoint(Vector2 ip){Vector2 min=pointCenter-staticSize;Vector2 max=pointCenter+staticSize;return ip>min&&ip<max;}int Point::pick(){std::chrono::duration<float>elapsed=std::chrono::high_resolution_clock::now()-generatedAt;return elapsed.count();}