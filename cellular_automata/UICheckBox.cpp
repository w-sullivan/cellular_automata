#include "UICheckBox.h"

//==========================(de)CONSTRUCTORS===================================

// Constructor for a UICheckBox.
//	x_loc:  x position on screen in pixels
//	y_loc:  y position on screen in pixels
//	text:  The default text on the checkbox's label
//	font:  Text font
//	text_color:  Text color
UICheckBox::UICheckBox(int x_loc, int y_loc, std::string text, TTF_Font* font, SDL_Color& textColor)
	: UILabel(text, font, textColor)
{
	_checkBox_rect.x = x_loc;
	_checkBox_rect.y = y_loc;
	_checkBox_rect.w = 20;
	_checkBox_rect.h = 20;

	_label_rect.x = _checkBox_rect.x + 28;
	_label_rect.y = _checkBox_rect.y + (_checkBox_rect.h / 2) - (_label_rect.h / 2);

	_enabled = false;
	_texture = nullptr;
	_enabled_texture = nullptr;

	buildCheckBoxTextures();
}


// UICheckBox destructor
UICheckBox::~UICheckBox()
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyTexture(_enabled_texture);
}


//=============================PUBLIC METHODS==================================

// Returns true if the given coordinates are within the checkbox
//	x:  x position in pixels
//	y:  y position in pixels
bool UICheckBox::clickedOn(int x, int y)
{
	if (x >= _checkBox_rect.x && x < _checkBox_rect.x + _checkBox_rect.w)
	{
		if (y >= _checkBox_rect.y && y < _checkBox_rect.y + _checkBox_rect.h)
		{
			return true;
		}
	}
	return false;
}


// Toggles the checkbox state, then returns current state.
bool UICheckBox::toggleEnabled()
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


// Draws the checkbox to the screen by sending it's texture to the renderer in
//	the Screen class. Overwrites the UILabel's draw method.
void UICheckBox::draw()
{
	if (_enabled)
	{
		SDL_RenderCopy(Screen::renderer, _enabled_texture, nullptr, &_checkBox_rect);
	}
	else
	{
		SDL_RenderCopy(Screen::renderer, _texture, nullptr, &_checkBox_rect);
	}

	SDL_RenderCopy(Screen::renderer, _label_texture, nullptr, &_label_rect);
}


//=============================PRIVATE METHODS=================================

// Creates two textures for the checkbox, an normal texture and an enabled 
//	texture.  Hardcoded. Creates texture from an array of integer color
//	values.  
bool UICheckBox::buildCheckBoxTextures()
{
	_texture = SDL_CreateTexture(Screen::renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		_checkBox_rect.w,
		_checkBox_rect.h);

	_enabled_texture = SDL_CreateTexture(Screen::renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		_checkBox_rect.w,
		_checkBox_rect.h);

	Uint32 borderColor = 0xFFFFFFFF;
	Uint32 backColor = 0x000000FF;
	Uint32 enabledColor = 0xFFFF00FF;

	Uint32* buffer = new Uint32[_checkBox_rect.w * _checkBox_rect.h];
	Uint32* enabled_buffer = new Uint32[_checkBox_rect.w * _checkBox_rect.h];
	
	int index = 0;

	for (int y = 0; y < _checkBox_rect.h; y++)
	{
		for (int x = 0; x < _checkBox_rect.w; x++)
		{
			if (y < 2 || y >= _checkBox_rect.h - 2)
			{
				buffer[index] = borderColor;
				enabled_buffer[index] = borderColor;
			}
			else if (x < 2 || x >= _checkBox_rect.w - 2)
			{
				buffer[index] = borderColor;
				enabled_buffer[index] = borderColor;
			}
			else
			{
				buffer[index] = backColor;

				// Draw the tic
				if (x >= 5 && x < _checkBox_rect.w - 5 && y >= 5 && y < _checkBox_rect.h - 5)
				{
					enabled_buffer[index] = enabledColor;
				}
				else
				{
					enabled_buffer[index] = backColor;
				}
			}
			index++;
		}
	}

	SDL_UpdateTexture(_texture, NULL, buffer, _checkBox_rect.w * sizeof(Uint32));
	SDL_UpdateTexture(_enabled_texture, NULL, enabled_buffer, _checkBox_rect.w * sizeof(Uint32));

	delete[] buffer;
	delete[] enabled_buffer;

	return true;
}