#ifndef RENDERER_H
#define RENDERER_H
#include<SDL2/SDL.h>
namespace Render{typedef unsigned char uChar;SDL_Renderer*get();void set(SDL_Renderer*gRenderer_);void setColor(uChar r,uChar g,uChar b);;void clearScreen();void render();void rect(SDL_Rect rect);void dot(int x,int y);void line(int x1,int y1,int x2,int y2);}
#endif