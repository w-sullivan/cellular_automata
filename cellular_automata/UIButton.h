#pragma once
#include "UILabel.h"

//=============================================================================
// UIButton Class:
// by William Sullivan
//
// The UIButton class is a simple implementation of a UI button.  When the
// clickedOn function is called it determines whether the given coordinates are 
// in the area it contains.  The colors, with exception of the font, and are
// hardcoded.  This button can also be used as a togglable button, by keeping 
// track of its enabled state.  It inherits from the UILabel class. Using the
// label as text for the button.  The draw function passes the appropriate
// texture to the Screen class's renderer.
//=============================================================================


class UIButton : public UILabel
{
public:
	UIButton(int x_loc, int y_loc,
			 int width, int height, 
			 std::string text, TTF_Font* font, SDL_Color& textColor);
	~UIButton();

	bool clickedOn(int x, int y);
	bool toggleEnabled();
	void setEnabled(bool enabled) { _enabled = enabled; }
	void draw();

	// Inherited from UILabel:
	// void setLabelText(std::string text);

protected:
	SDL_Rect _button_rect;					// The button's size and location
	SDL_Texture* _texture;					// The button's texture
	SDL_Texture* _enabled_texture;			// The button's texture if togglable
	bool _enabled;							// Whether a togglable button is 
											//   enabled or not

	// Inherited from UILabel:
	//	SDL_Rect _label_rect;				// Label's area and location
	//	std::string _text;					// Label's text to display
	//	TTF_Font* _font;					// Label's text font
	//	int _font_size;						// Label's text font size
	//	SDL_Color _text_color;				// Label's text font color
	//	SDL_Texture* _label_texture;		// Label's texture

private:
	bool buildButtonTextures();
};

