#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Screen.h"

//=============================================================================
// UILabel Class:
// by William Sullivan
//
// The UILabel class defines a label similar to those found in other languages
// such as C# and visual basic.  It defines a rectangular area that contains
// text.  The label's text can be changed.  It is drawn to the screen by
// accessing the renderer found in the Screen class.
//
// The UILabel class can be inherited to allow the creation of other UI
// elements that require an area and text, such as a button, check boxes, or
// textboxes.
//=============================================================================


class UILabel
{
public:
	UILabel(int x_loc, int y_loc, std::string text, TTF_Font* font, SDL_Color& text_color);
	UILabel(std::string text, TTF_Font* font, SDL_Color& text_color);
	void setLabelText(std::string text);
	void draw();
	~UILabel();

protected:
	SDL_Rect _label_rect;					// The area and location
	std::string _text;						// Label text to display
	TTF_Font* _font;						// Label's text font
	SDL_Color _text_color;					// Label's text font color
	SDL_Texture* _label_texture;			// Label's texture
};

