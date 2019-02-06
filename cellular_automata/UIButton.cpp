#include "UIButton.h"

//==========================(de)CONSTRUCTORS===================================

// Constructor for a UIButton.
//	x_loc:  x position on screen in pixels
//	y_loc:  y position on screen in pixels
//	width:  Button width in pixels
//	height:  Button height in pixels
//	text:  The text displayed on the button
//	font:  button text font
//	text_color:  button text color
UIButton::UIButton(int x_loc, int y_loc, int width, int height, 
				   std::string text, TTF_Font* font, SDL_Color& textColor)
				   : UILabel(text, font, textColor)
{
	_button_rect.x = x_loc;
	_button_rect.y = y_loc;
	_button_rect.w = width;
	_button_rect.h = height;

	// Set the button's label to be centered in the button
	_label_rect.x = _button_rect.x + (_button_rect.w / 2) - (_label_rect.w / 2);
	_label_rect.y = _button_rect.y + (_button_rect.h / 2) - (_label_rect.h / 2);

	_enabled = false;
	_texture = nullptr;
	_enabled_texture = nullptr;

	buildButtonTextures();
}


// UIButton Destuctor
UIButton::~UIButton()
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyTexture(_enabled_texture);
}


//=============================PUBLIC METHODS==================================

// Returns true if the given coordinates are within the button
//	x:  x position in pixels
//	y:  y position in pixels
bool UIButton::clickedOn(int x, int y)
{
	if (x >= _button_rect.x && x < _button_rect.x + _button_rect.w)
	{
		if (y >= _button_rect.y && y < _button_rect.y + _button_rect.h)
		{
			return true;
		}
	}

	return false;
}


// Toggles the buttons state, then returns current state.
bool UIButton::toggleEnabled()
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


// Draws the button to the screen by sending it's texture to the renderer in
//	the Screen class. Overwrites the UILabel's draw method.
void UIButton::draw()
{
	if (_enabled)
	{
		SDL_RenderCopy(Screen::renderer, _enabled_texture, nullptr, &_button_rect);
	}
	else
	{
		SDL_RenderCopy(Screen::renderer, _texture, nullptr, &_button_rect);
	}

	SDL_RenderCopy(Screen::renderer, _label_texture, nullptr, &_label_rect);
}


//=============================PRIVATE METHODS=================================

// Creates two textures for the button, an normal texture and an enabled 
//	texture.  Hardcoded. Creates texture from an array of integer color
//	values.  
bool UIButton::buildButtonTextures()
{
	_texture = SDL_CreateTexture(Screen::renderer, 
								 SDL_PIXELFORMAT_RGBA8888, 
								 SDL_TEXTUREACCESS_STATIC, 
								 _button_rect.w, 
								 _button_rect.h);

	_enabled_texture = SDL_CreateTexture(Screen::renderer, 
										 SDL_PIXELFORMAT_RGBA8888, 
										 SDL_TEXTUREACCESS_STATIC,
										 _button_rect.w, 
										 _button_rect.h);

	Uint32 borderColor = 0x646464FF;
	Uint32 backColor = 0xBEBEBEFF;
	Uint32 enabledColor = 0xFFFF00FF;

	Uint32* buffer = new Uint32[_button_rect.w * _button_rect.h];
	Uint32* enabled_buffer = new Uint32[_button_rect.w * _button_rect.h];
	int index = 0;

	for (int y = 0; y < _button_rect.h; y++)
	{
		for (int x = 0; x < _button_rect.w; x++)
		{
			if (y <= 2 || y >= _button_rect.h - 3)
			{
				buffer[index] = borderColor;
				enabled_buffer[index] = borderColor;
			}
			else if (x <= 2 || x >= _button_rect.w - 3)
			{
				buffer[index] = borderColor;
				enabled_buffer[index] = borderColor;
			}
			else
			{
				buffer[index] = backColor;
				enabled_buffer[index] = enabledColor;
			}
			index++;
		}
	}

	SDL_UpdateTexture(_texture, NULL, buffer, _button_rect.w * sizeof(Uint32));
	SDL_UpdateTexture(_enabled_texture, NULL, enabled_buffer, _button_rect.w * sizeof(Uint32));

	delete[] buffer;
	delete[] enabled_buffer;

	return true;
}
