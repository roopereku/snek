#include "Window.h"
#include "Renderer.h"

Window::Window(const char* name, bool& success, bool hasVsync, int sizeDivX, int sizeDivY)
{
	// Success is set to false to shorten code
	success = false;

	// Get the monitor resolution
	SDL_DisplayMode dMode;
	SDL_GetCurrentDisplayMode(0, &dMode);

	// Set width and height
	width = dMode.w / sizeDivX;
	height = dMode.h / sizeDivY;

	// Create a window and do error checking
	gWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);

	if(gWindow == nullptr)
		SDL_Log("Unable to create a window : %s!", SDL_GetError());

	else
	{
		// Create a renderer and do error checking
		auto renderFlags = SDL_RENDERER_ACCELERATED | (hasVsync ? SDL_RENDERER_PRESENTVSYNC : 0);
		gRenderer = SDL_CreateRenderer(gWindow, -1, renderFlags);

		if(gRenderer == nullptr)
			SDL_Log("Unable to create a renderer : %s!", SDL_GetError());

		else
		{
			// Set ths renderer as the current one
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
		// If windows is closed, set running to false
		case SDL_QUIT: running = false; break;

		// If the window is resized, adjust width and height
		case SDL_WINDOWEVENT:
			if(evnt.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(gWindow, &width, &height);
			}
		break;
	}
}
