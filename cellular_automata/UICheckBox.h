#pragma once
#include "UILabel.h"

//=============================================================================
// UICheckBox Class:
// by William Sullivan
//
// The UICheckBox class is a simple implementation of a UI checkbox.  It
// inherits from the label class to manage the text.  In addition to text, it
// adds the checkbox to the left of the text.  The checkbox keeps track of its
// state.  When initialized the checkbox draws its own texture and enabled
// texture.  The draw function returns the appropriate texture to the Screen
// class's renderer.  The checkbox size and color can not be changed.
//=============================================================================

class UICheckBox : public UILabel
{
public:
	UICheckBox(int x_loc, int y_loc, std::string text, TTF_Font* font, SDL_Color& textColor);
	~UICheckBox();
	bool clickedOn(int x, int y);
	bool toggleEnabled();
	void draw();
	
	// Inherited from UILabel:
	// void setLabelText(std::string text);

private:
	SDL_Rect _checkBox_rect;				// The checkbox's location and dimensions
	SDL_Texture* _texture;					// The checkbox's texture
	SDL_Texture* _enabled_texture;			// The checkbox's enabled texture. (with tick)
	bool _enabled;							// Describes if checkbox is enabled (ticked)

	bool buildCheckBoxTextures();

//	Inherited from UILabel:
//	SDL_Rect _label_rect;					// Label's area and location
//	std::string _text;						// Label's text to display
//	TTF_Font* _font;						// Label's text font
//	int _font_size;							// Label's text font size
//	SDL_Color _text_color;					// Label's text font color
//	SDL_Texture* _label_texture;			// Label's texture
};

