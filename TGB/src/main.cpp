#include <SDL\SDL.h>
#include "Platform.h"
#include "CPU.h"

int main(int argc, char* argv[]) {
	Platform* platform = new Platform("TGB - GameBoy", 160, 144, 1);
	CPU gameboy;

	while (!platform->events()) {
		gameboy.update();
		platform->update();
	}
	return 0;
}