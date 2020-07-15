#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <SDL\SDL_video.h>
#include <SDL\SDL_render.h>

class Platform
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Platform(const char* title, int width, int height, int scale);
	~Platform();

	bool events();
	void update();

};

#endif // !_PLATFORM_H_
