#include "Window.h"
#include "Renderer.h"

Window::Window(const char* name, bool& success, bool hasVsync, int sizeDivX, int sizeDivY)
{
	success = false;

	SDL_DisplayMode dMode;
	SDL_GetCurrentDisplayMode(0, &dMode);

	width = dMode.w / sizeDivX;
	height = dMode.h / sizeDivY;

	gWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);

	if(gWindow == nullptr)
		SDL_Log("Unable to create a window : %s!", SDL_GetError());

	else
	{
		auto renderFlags = SDL_RENDERER_ACCELERATED | (hasVsync ? SDL_RENDERER_PRESENTVSYNC : 0);
		gRenderer = SDL_CreateRenderer(gWindow, -1, renderFlags);

		if(gRenderer == nullptr)
			SDL_Log("Unable to create a renderer : %s!", SDL_GetError());

		else
		{
			Render::set(gRenderer);

			success = true;
		}
	}
}

Window::~Window()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
}

int Window::operator[](const char type)
{
	switch(type)
	{
		case 'W': return width;
		case 'H': return height;
	}

	return 0;
}

void Window::input(SDL_Event evnt, bool& running)
{
	switch(evnt.type)
	{
		case SDL_QUIT: running = false; break;

		case SDL_WINDOWEVENT:
			if(evnt.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(gWindow, &width, &height);
			}
		break;
	}
}
