#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <time.h>

#include "Options.h"
#include "Screen.h"
#include "Cell.h"

//=============================================================================
// World class:
// by William Sullivan
//
// Divides the screen into blocks of cells
// Manages location and interaction of cells
//=============================================================================

class World
{
private:
	// Member variables
	const static int CELL_COUNT_X = 192;	//-Number of cells in each row
	const static int CELL_COUNT_Y = 192;	//-Number of rows of cells

	SDL_Rect* _world_space;
	Uint32* _pixel_buffer;
	SDL_Texture* _texture;					//-The texture for rendering the world

	// The block of cells
	Cell* cells[CELL_COUNT_Y][CELL_COUNT_X];

	// Private methods
	void setNeighborhood(int cell_x, int cell_y);
	Uint32 getPixelColor(int x, int y);
	Uint32 getReproductionColor(Cell* parent1, Cell* parent2);
	std::string toBinary32(Uint32 num);
	Uint32 fromBinary(std::string num);

public:

	// Constructors
	World(int x_loc, int y_loc, int width, int height);

	void erase(int screen_x, int screen_y);
	void draw();
	void update();
	void clear();
	void forceLiving(int type, int screen_x, int screen_y);
	void moveCell(Cell* start_location, Cell* new_location);
	void reproduce(int cell_x, int cell_y);
	
	Cell* find_food(Cell* animal);
	bool inWorld(int x, int y);

	~World();
};