#include "Screen.h"

SDL_Renderer *Screen::renderer = nullptr;

// =============================Private=====================================

bool Screen::_init(int x, int y)
{

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << "SDL initialized..." << std::endl;
	}
	else
	{
		std::cerr << "SDL failed to initialize!" << std::endl;
		return false;
	}


	// Create Window
	_window = SDL_CreateWindow("Cellular Automata", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screen_width, _screen_height, SDL_WINDOW_SHOWN);
	if (_window)
	{
		std::cout << "Window created..." << std::endl;
	}
	else
	{
		std::cerr << "Window failed to initialize!" << std::endl;
		return false;
	}


	// Create Renderer
	renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		std::cout << "Renderer created..." << std::endl;
	}
	else
	{
		std::cerr << "Renderer failed to initialize!" << std::endl;
		return false;
	}

	return true;
}


//==============================Public======================================

// Constructor for a Screen class.
// * x: Screen width in pixels.
// * y: Screen height in pixels.
Screen::Screen(int x, int y)
{
	_screen_width = x;
	_screen_height = y;
	_window = nullptr;

	if (_init(x, y) != false)
		_is_running = true;
	else
		_is_running = false;
}


// Screen destructor
Screen::~Screen()
{
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(_window);
	
	SDL_Quit();
	std::cout << "SDL cleaned up..." << std::endl;
}

