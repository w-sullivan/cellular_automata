#include "UILabel.h"

//========================(de)CONSTRUCTORS=====================================

// Constructor for a UILabel:
//	x_loc:  x position on screen in pixels
//	y_loc:  y position on screen in pixels
//	text:  label text to be displayed
//	font:  label text font
//	text_color:  label text color
UILabel::UILabel(int x_loc, int y_loc, std::string text, TTF_Font* font, SDL_Color& text_color)
{
	_label_rect.x = x_loc;
	_label_rect.y = y_loc;

	_text = text;
	_font = font;
	_text_color = text_color;

	_label_texture = nullptr;

	setLabelText(text);
}


// Constructor for a UILabel (default position and size)
//	text:  label text to be displayed
//	font:  label text font
//	text_color:  label text color
UILabel::UILabel(std::string text, TTF_Font* font, SDL_Color& text_color)
{
	_label_rect.x = 0;
	_label_rect.y = 0;

	_text = text;
	_font = font;
	_text_color = text_color;

	_label_texture = nullptr;

	setLabelText(text);
}


// Destructor
UILabel::~UILabel()
{
}



//=============================PUBLIC METHODS==================================

// Sets the label's text with given string, recreates label texture.
//	text:  new label text
void UILabel::setLabelText(std::string text)
{
	// Update the label's text
	_text = text;

	// Create labels new texture
	SDL_Surface* text_surface = TTF_RenderText_Blended(_font, _text.c_str(), _text_color);
	_label_texture = SDL_CreateTextureFromSurface(Screen::renderer, text_surface);
	SDL_FreeSurface(text_surface);

	// Adjust the labels width and height to the new textures dimensions 
	SDL_QueryTexture(_label_texture, nullptr, nullptr, &_label_rect.w, &_label_rect.h);
}


// Sends the label to Class Screen to be rendered
void UILabel::draw()
{
	SDL_RenderCopy(Screen::renderer, _label_texture, nullptr, &_label_rect);
}