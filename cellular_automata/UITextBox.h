#pragma once
#include <iostream>
#include <SDL.h>
#include "UILabel.h"

//=============================================================================
// UITextBox Class:
// by William Sullivan
//
// The UITextBox class is a simple implementation of a textbox.  It inherits
// from the UILabel class to handle the text display.  It works similar to the
// UIButtton, in that it can be enabled and disabled.  It generates its own
// texture and enabled texture on initialization.  The draw function passes
// the appropriate texture to the Screen class's renderer.
//=============================================================================


class UITextBox : public UILabel
{
public:
	UITextBox(int x_loc, int y_loc, int width, int height, std::string text, TTF_Font* font, SDL_Color& textColor);
	~UITextBox();
	bool clickedOn(int x, int y);
	bool toggleEnabled();
	void setEnabled(bool enabled) { _enabled = enabled; }
	void draw();

	// Inherited from UILabel:
	// void setLabelText(std::string text);

private:
	SDL_Rect _box_rect;						// The textbox's area and location
	SDL_Texture* _box_texture;				// The textbox's texture
	SDL_Texture* _enabled_texture;			// The textbox's enabled texture
	bool _enabled;							// Describes if enabled or not.

	bool buildTextBoxTexture();

	// Inherited from UILabel:
	//	SDL_Rect _label_rect;				// Label's area and location
	//	std::string _text;					// Label's text to display
	//	TTF_Font* _font;					// Label's text font
	//	int _font_size;						// Label's text font size
	//	SDL_Color _text_color;				// Label's text font color
	//	SDL_Texture* _label_texture;		// Label's texture
};

