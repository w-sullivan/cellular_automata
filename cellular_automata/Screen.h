#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>


class Screen
{
private:
	// Member Variables
	int _screen_width;
	int _screen_height;
	SDL_Window* _window;
	bool _is_running;

	// Private Methods
	bool _init(int x, int y);

public:
	
	static SDL_Renderer* renderer;

	// Public Methods
	Screen(int x, int y);
	~Screen();
	void quit() { _is_running = false; }
	bool running() { return _is_running; }
};