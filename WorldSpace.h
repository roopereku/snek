#ifndef WORLD_SPACE_H
#define WORLD_SPACE_H
#include "Window.h"
#include "Vector2.h"
#include<SDL2/SDL.h>
class WorldSpace{public:WorldSpace(Window&win);void draw();float&operator[](int index);Vector2 toScreen(Vector2 vec);SDL_Rect rectToScreen(Vector2 position,Vector2 size);private:float radiusX;float radiusY;Window&win;};
#endif