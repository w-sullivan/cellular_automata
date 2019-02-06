#include "Panel.h"



Panel::Panel(int x_pos, int y_pos, int width, int height, SDL_Renderer* renderer)
{

	_rect = new SDL_Rect();
	_rect->x = x_pos;
	_rect->y = y_pos;
	_rect->w = width;
	_rect->h = height;

}


Panel::~Panel()
{
	delete _rect;
}
