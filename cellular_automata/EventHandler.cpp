#include "EventHandler.h"



EventHandler::EventHandler(Screen* screen, World* world, Options* options)
{
	_screen = screen;
	_world = world;
	_options = options;
	_text_edit_mode = false;
	//_event = NULL;

}


void EventHandler::pollEvents()
{
	// If an Event occured
	if (SDL_PollEvent(&_event))
	{
		// Text Edit Mode
		if (_text_edit_mode)
		{
			textEdit();
		}
		// Normal Mode
		else
		{
			switch (_event.type)
			{
			case SDL_QUIT:
				_screen->quit();
				break;
			case SDL_MOUSEBUTTONDOWN:
				handleMouseClick();
				break;
			case SDL_KEYDOWN:
				switch (_event.key.keysym.sym)
				{
				case SDLK_SPACE:
					_options->togglePause();
					break;
				case SDLK_ESCAPE:
					_screen->quit();
					break;
				}
				break;
			}
		}
	}
}


void EventHandler::handleMouseClick()
{
	// Get the mouse position
	int x, y;
	SDL_GetMouseState(&x, &y);

	// Debug: output mouse position
	//std::cout << "[" << x << ", " << y << "]" << std::endl;

	// Check which panel was clicked.
	if (_world->inWorld(x, y))
	{
		worldClick(x, y);
	}
	else if (_options->clickAt(x, y))
	{
		if (_options->editingText())
			_text_edit_mode = true;

		if (_options->quit())
			_screen->quit();

		if (_options->clear())
			_world->clear();
	}
}


void EventHandler::worldClick(int x, int y)
{
	// From Options, find which action to take.

	// If delete
	if (_options->getCreationType() == 0)
	{
		_world->erase(x, y);
	}
	// If create
	else
	{
		// World should create an entity of given type at mouse x, y
		_world->forceLiving(_options->getCreationType(), x, y);
	}
}


void EventHandler::textEdit()
{
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		switch (_event.key.keysym.sym)
		{
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			_options->stopEditingText();
			_text_edit_mode = false;
			break;
		case SDLK_BACKSPACE:
			_options->textboxBackspace();
			break;
		case SDLK_0: 
		case SDLK_KP_0:
			_options->addToTextbox('0');
			break;
		case SDLK_1:
		case SDLK_KP_1:
			_options->addToTextbox('1');
			break;
		case SDLK_2:
		case SDLK_KP_2:
			_options->addToTextbox('2');
			break;
		case SDLK_3:
		case SDLK_KP_3:
			_options->addToTextbox('3');
			break;
		case SDLK_4:
		case SDLK_KP_4:
			_options->addToTextbox('4');
			break;
		case SDLK_5:
		case SDLK_KP_5:
			_options->addToTextbox('5');
			break;
		case SDLK_6:
		case SDLK_KP_6:
			_options->addToTextbox('6');
			break;
		case SDLK_7:
		case SDLK_KP_7:
			_options->addToTextbox('7');
			break;
		case SDLK_8:
		case SDLK_KP_8:
			_options->addToTextbox('8');
			break;
		case SDLK_9:
		case SDLK_KP_9:
			_options->addToTextbox('9');
			break;
		}
		break;
	}
}
