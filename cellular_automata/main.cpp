// William Sullivan

#include <iostream>
#include "Screen.h"
#include "World.h"
#include "Options.h"
#include "EventHandler.h"

// Resolution
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int FRAME_RATE = 60;


int main(int argc, char* argv[])
{
	Screen* screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	World* world = new World(0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT);
	Options* options = new Options(SCREEN_HEIGHT, 0, SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_HEIGHT);
	EventHandler* event_handler = new EventHandler(screen, world, options);
	
	// Limiting frame rate
	const int frame_delay = 1000 / FRAME_RATE;
	Uint32 frame_start;
	int frame_time;


	std::cout << "Begining main loop" << std::endl;
	// While simulation is running
	while (screen->running())
	{
		frame_start = SDL_GetTicks();

		// Poll Events
		event_handler->pollEvents();

		// Update World logic
		if(options->paused() == false)
		{
			world->update();
		}

		// Clear Renderer
		SDL_RenderClear(Screen::renderer);

		// Update Textures
		world->draw();
		options->draw();

		// Render to screen
		SDL_RenderPresent(Screen::renderer);

		// Frame Rate cap
		frame_time = SDL_GetTicks() - frame_start;
		if (frame_time > frame_delay)
		{
			SDL_Delay(frame_time - frame_delay);
		}
	}

	return 0;
}