#pragma once
#include <SDL.h>
#include "Screen.h"
#include "World.h"
#include "Options.h"

class EventHandler
{
public:
	EventHandler(Screen* screen, World* world, Options* options);

	void pollEvents();
	void handleMouseClick();
	void textEdit();
	void worldClick(int x, int y);

private:
	SDL_Event _event;
	Screen* _screen;
	World*	_world;
	Options* _options;
	bool _text_edit_mode;

};

