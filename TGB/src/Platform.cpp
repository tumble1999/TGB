#include "Platform.h"]

#include <SDL\SDL.h>

Platform::Platform(const char* title, int width, int height, int scale)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * scale, height * scale, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Platform::~Platform()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Platform::events()
{
	bool quit = false;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
		{
			quit = true;
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				quit = true;
			} break;
			//Setup Input
			}
		} break;

		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			//Setup Input
			}
			break;
		}
		default:
			break;
		}
	}
	return quit;
}

void Platform::update()
{
}
