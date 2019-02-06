#include "UITextBox.h"

//==========================(de)CONSTRUCTORS===================================

// Constructor for a UITextBox.
//	x_loc:  x position on screen in pixels
//	y_loc:  y position on screen in pixels
//	width:  Textbox width in pixels
//	height:  Textbox height in pixels
//	default_text:  The default text in the textbox
//	font:  Text font
//	text_color:  Text color
UITextBox::UITextBox(int x_loc, int y_loc, int width, int height, std::string default_text, TTF_Font* font, SDL_Color& textColor)
	: UILabel(default_text, font, textColor)
{
	// Set location and dimensions
	_box_rect.x = x_loc;
	_box_rect.y = y_loc;
	_box_rect.w = width;
	_box_rect.h = height;

	// Place text area in the middle vertically, and to the left
	_label_rect.x = _box_rect.x + 5;
	_label_rect.y = _box_rect.y + (_box_rect.h / 2) - (_label_rect.h / 2);

	_text = default_text;

	_box_texture = nullptr;
	_enabled_texture = nullptr;

	buildTextBoxTexture();
}


// UITextBox Destructor
UITextBox::~UITextBox()
{
	SDL_DestroyTexture(_box_texture);
	SDL_DestroyTexture(_enabled_texture);

}


//=============================PUBLIC METHODS==================================

// Returns true if the given coordinates are within the textbox
//	x:  x position in pixels
//	y:  y position in pixels
bool UITextBox::clickedOn(int x, int y)
{
	if (x >= _box_rect.x && x < _box_rect.x + _box_rect.w)
	{
		if (y >= _box_rect.y && y < _box_rect.y + _box_rect.h)
		{
			return true;
		}
	}

	return false;
}


// Toggles a textbox's state, then returns current state.
bool UITextBox::toggleEnabled()
{
	if (_enabled)
	{
		_enabled = false;
		return _enabled;
	}
	else
	{
		_enabled = true;
		return _enabled;
	}
}


// Draws the textbox to the screen by sending it's texture to the renderer in
//	the Screen class. Overwrites the UILabel's draw method.
void UITextBox::draw()
{
	if (_enabled)
	{
		SDL_RenderCopy(Screen::renderer, _enabled_texture, nullptr, &_box_rect);
	}
	else
	{
		SDL_RenderCopy(Screen::renderer, _box_texture, nullptr, &_box_rect);
	}

	SDL_RenderCopy(Screen::renderer, _label_texture, nullptr, &_label_rect);
}


//=============================PRIVATE METHODS=================================

// Creates two textures for the textbox, an normal texture and an enabled 
//	texture.  Hardcoded. Creates texture from an array of integer color
//	values.  
bool UITextBox::buildTextBoxTexture()
{
	_box_texture = SDL_CreateTexture(Screen::renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		_box_rect.w,
		_box_rect.h);

	_enabled_texture = SDL_CreateTexture(Screen::renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		_box_rect.w,
		_box_rect.h);

	Uint32 borderColor = 0xFFFFFFFF;
	Uint32 backColor = 0x000000FF;
	Uint32 enabled_background = 0x464646FF;

	Uint32* buffer = new Uint32[_box_rect.w * _box_rect.h];
	Uint32* enabled_buffer = new Uint32[_box_rect.w * _box_rect.h];
	int index = 0;

	for (int y = 0; y < _box_rect.h; y++)
	{
		for (int x = 0; x < _box_rect.w; x++)
		{
			if (y <= 1 || y >= _box_rect.h - 2)
			{
				buffer[index] = borderColor;
				enabled_buffer[index] = borderColor;
			}
			else if (x <= 1 || x >= _box_rect.w - 2)
			{
				buffer[index] = borderColor;
				enabled_buffer[index] = borderColor;
			}
			else
			{
				buffer[index] = backColor;
				enabled_buffer[index] = enabled_background;
			}
			index++;
		}
	}

	SDL_UpdateTexture(_box_texture, NULL, buffer, _box_rect.w * sizeof(Uint32));
	SDL_UpdateTexture(_enabled_texture, NULL, enabled_buffer, _box_rect.w * sizeof(Uint32));

	delete[] buffer;
	delete[] enabled_buffer;

	return true;
}
