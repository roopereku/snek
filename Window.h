#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
public:
	Window(const char* name, bool& success, bool hasVsync, int sizeDivX = 2, int sizeDivY = 2);
	~Window();

	int operator[](const char type);
	void input(SDL_Event evnt, bool& running);

private:
	int width;
	int height;

	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
};

#endif
