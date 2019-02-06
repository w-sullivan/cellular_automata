#include "World.h"

//====================CONSTRUCTORS & DESTRUCTORS===============================

// World constructor.  Creates a 2d array of cell objects. And creates each 
// cell's neighborhood.
World::World(int x_loc, int y_loc, int width, int height)
{
	srand(time(0));

	_world_space = new SDL_Rect();

	_world_space->x = x_loc;
	_world_space->y = y_loc;
	_world_space->w = width;
	_world_space->h = height;

	// Create cells
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			cells[y][x] = new Cell();
		}
	}

	// Set Neighbors
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			setNeighborhood(x, y);
		}
	}

	// Initialize pixel buffer.
	_pixel_buffer = new Uint32[_world_space->w * _world_space->h];

	// Create World texture
	_texture = SDL_CreateTexture(Screen::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
	if (_texture)
	{
		std::cout << "World texture created..." << std::endl;
	}
	else
	{
		std::cerr << "World texture failed to be created!" << std::endl;
	}
}


// Deconstructor
World::~World()
{
	delete _world_space;
	delete _pixel_buffer;

	// delete texture
	SDL_DestroyTexture(_texture);

	// Delete cells
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			delete cells[y][x];
		}
	}

	std::cout << "Cells deleted..." << std::endl;
}



//===========================PUBLIC METHODS====================================

void World::erase(int screen_x, int screen_y)
{
	int x_index = screen_x / (_world_space->w / CELL_COUNT_X);
	int y_index = screen_y / (_world_space->h / CELL_COUNT_Y);

	cells[y_index][x_index]->die();
}


void World::draw()
{
	int buffer_index = 0;

	for (int y = 0; y < _world_space->h; y++)
	{
		for (int x = 0; x < _world_space->w; x++)
		{
			_pixel_buffer[buffer_index] = getPixelColor(x, y);
			buffer_index++;
		}
	}

	SDL_UpdateTexture(_texture, NULL, _pixel_buffer, _world_space->w * sizeof(Uint32));
	SDL_RenderCopy(Screen::renderer, _texture, NULL, _world_space);
}


// Return a cells color for a given pixel location.
Uint32 World::getPixelColor(int x, int y)
{
	// Find the cell
	int x_index = float(x) * float(float(CELL_COUNT_X) / float(_world_space->w));
	int y_index = float(y) * float(float(CELL_COUNT_Y) / float(_world_space->h));

	// Return its color
	return cells[y_index][x_index]->myColor();
}


// Updates the world
void World::update()
{
	int animal_count = 0;
	// For every cell
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			// If a living cell and can act
			if (cells[y][x]->isAlive() && cells[y][x]->canAct())
			{
				// Try to reproduce
				reproduce(x, y);

				// Plants
				if (cells[y][x]->isPlant())
				{
					// Increase energy (photosythesis?)
					cells[y][x]->increaseEnergy(Options::getGrowthRate());

					if (Options::getAgeDeath())
					{
						cells[y][x]->increaseAge(1);
					}
				}
				// Animals
				else
				{
					cells[y][x]->decreaseEnergy(1);
					// Move and eat food
					Cell* food = find_food(cells[y][x]);
					if (food)
					{
						// Move cell onto the food cell.
						moveCell(cells[y][x], food);
						food->decreaseEnergy(1);
					}
					// Else, move randomly
					else
					{
						// move in random direction
						int random_direction = rand() % 8;
						bool clockwise;
						if (rand() % 2)
							clockwise = true;
						else
							clockwise = false;
						
						// Wait near food or move to empty space
						Cell* move_location = cells[y][x]->findEmpty(random_direction, clockwise);

						// If there is a place to move, move.
						if (move_location)
						{
							moveCell(cells[y][x], move_location);
						}
					}
				}
			}
			// If alive, but cannot act.
			else if (cells[y][x]->isAlive())
			{
				cells[y][x]->make_active();
			}
		}
	}
} // end update


void World::clear()
{
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			cells[y][x]->die();
		}
	}
}


Uint32 World::getReproductionColor(Cell* parent1, Cell* parent2)
{
	// convert the color to a string representation of its binary form
	std::string parent1_color = toBinary32(parent1->myColor());
	std::string parent2_color = toBinary32(parent2->myColor());

	std::string child_color = "";

	// Insert half of parent1's color, with mutation chance
	for (int i = 0; i < 12; i++)
	{
		if (rand() % Options::getMutationRate() > 1)
		{
			child_color += parent1_color[i];
		}
		// Mutate (bit flip)
		else
		{
			if (parent1_color[i] == '1')
			{
				child_color += '0';
			}
			else
			{
				child_color += '1';
			}
		}
	}

	// Insert half of parent2's color, with mutation chance.
	for (int i = 12; i < 24; i++)
	{
		if (rand() % Options::getMutationRate() > 1)
		{
			child_color += parent2_color[i];
		}
		// Mutate
		else
		{
			if (parent2_color[i] == '1')
			{
				child_color += '0';
			}
			else
			{
				child_color += '1';
			}
		}
	}
	// Insert Alpha value.
	child_color += "11111111";

	// Convert childs color back to Uint32 and return.
	return fromBinary(child_color);
}


// Make a given cell alive, at screen position.
void World::forceLiving(int type, int screen_x, int screen_y)
{
	// Find the cell
	int x_index = screen_x / (_world_space->w / CELL_COUNT_X);
	int y_index = screen_y / (_world_space->h / CELL_COUNT_Y);

	cells[y_index][x_index]->birth(type);
}


void World::moveCell(Cell* start_location, Cell* new_location)
{
	new_location->equals(start_location);
	start_location->die();
}


void World::reproduce(int cell_x, int cell_y)
{
	// If the cell has enough energy to reproduce
	if (cells[cell_y][cell_x]->canReproduce())
	{
		Cell* mate = nullptr;

		// Initialize random'ish search
		int start_index = rand() % 8;  // random start index between 0-7

		bool clockwise;  // random search direction. If true, clockwise. Else counter-clockwise
		if (rand() % 2 == 0)
			clockwise = true;
		else
			clockwise = false;


		// DEBUG
		if (cells[cell_y][cell_x]->isAlive() == false)
		{
			std::cout << "Dead cell can reproduce" << std::endl;
		}
		// Case: PLANT
		else if (cells[cell_y][cell_x]->isPlant())
		{
			mate = cells[cell_y][cell_x]->findMate(start_index, clockwise);
		}
		// Case: ANIMAL
		else if (cells[cell_y][cell_x]->isAnimal())
		{
			mate = cells[cell_y][cell_x]->findMate(start_index, clockwise);
		}
		// DEBUG
		else
		{
			std::cout << "Found cell that is alive and not a plant or animal." << std::endl;
		}

		// If they can find a mate
		if (mate)
		{
			// Get new random'ish search start and direction
			start_index = rand() % 8;  // random start index between 0-7

			// random search direction.
			if (rand() % 2 == 0)
				clockwise = true;
			else
				clockwise = false;

			// Find a place for the new cell to be born.
			// Plants need an empty cell.
			// Animals can be born on empty or plant cells
			Cell* birth_location = nullptr;

			// DEBUG
			if (cells[cell_y][cell_x]->isAlive() == false)
			{
				std::cout << "A dead cell is somehow breeding... zombies!!" << std::endl;
			}
			// Case: PLANT
			else if (cells[cell_y][cell_x]->isPlant())
			{
				birth_location = cells[cell_y][cell_x]->findEmpty(start_index, clockwise);
			}
			// Case: Animal
			else if (cells[cell_y][cell_x]->isAnimal())
			{
				birth_location = cells[cell_y][cell_x]->findEmpty(start_index, clockwise);
			}

			// If there is an empty place
			if (birth_location)
			{
				birth_location->birth(cells[cell_y][cell_x]->getType(), getReproductionColor(cells[cell_y][cell_x], mate));

				// decrease the parents energy
				cells[cell_y][cell_x]->decreaseEnergy(50);
				mate->decreaseEnergy(50);
			}
		}
	}
}


Cell* World::find_food(Cell* animal)
{
	// Look for food (randomly)
	int start_index = rand() % 8;  // random start index between 0-7

	bool clockwise;  // random search direction. If true, clockwise. Else counter-clockwise
	if (rand() % 2 == 0)
		clockwise = true;
	else
		clockwise = false;

	// If they can find food
	return animal->findFood(start_index, clockwise, Options::getAnimalPref());
}


bool World::inWorld(int x, int y)
{
	if (x >= _world_space->x && x < _world_space->w)
	{
		if (y >= _world_space->y && y < _world_space->h)
		{
			return true;
		}
	}

	return false;
}



//========================PRIVATE METHODS =====================================

// Given a cell at cell index, set that cells
//	neighbors to a created list of neighbors
void World::setNeighborhood(int cell_x, int cell_y)
{
	Cell* neighbors[8];

	// 0: North
	if (cell_y - 1 >= 0)
		neighbors[0] = cells[cell_y - 1][cell_x];
	else
		neighbors[0] = nullptr;

	// 1: Northeast
	if (cell_y - 1 >= 0 && cell_x + 1 < CELL_COUNT_X)
		neighbors[1] = cells[cell_y - 1][cell_x + 1];
	else
		neighbors[1] = nullptr;

	// 2: East
	if (cell_x + 1 < CELL_COUNT_X)
		neighbors[2] = cells[cell_y][cell_x + 1];
	else
		neighbors[2] = nullptr;

	// 3: Southeast
	if (cell_y + 1 < CELL_COUNT_Y && cell_x + 1 < CELL_COUNT_X)
		neighbors[3] = cells[cell_y + 1][cell_x + 1];
	else
		neighbors[3] = nullptr;

	// 4: South
	if (cell_y + 1 < CELL_COUNT_Y)
		neighbors[4] = cells[cell_y + 1][cell_x];
	else
		neighbors[4] = nullptr;

	// 5: Southwest
	if (cell_y + 1 < CELL_COUNT_Y && cell_x - 1 >= 0)
		neighbors[5] = cells[cell_y + 1][cell_x - 1];
	else
		neighbors[5] = nullptr;

	// 6: West
	if (cell_x - 1 >= 0)
		neighbors[6] = cells[cell_y][cell_x - 1];
	else
		neighbors[6] = nullptr;

	// 7: Northwest
	if (cell_y - 1 >= 0 && cell_x - 1 >= 0)
		neighbors[7] = cells[cell_y - 1][cell_x - 1];
	else
		neighbors[7] = nullptr;

	// Set the cells neighborhood.
	cells[cell_y][cell_x]->setNeighbors(neighbors);
}


std::string World::toBinary32(Uint32 num)
{
	std::string result = "";
	int remainder;
	int div = num;
	int count = 0;

	while (count < 32)
	{
		remainder = div % 2;
		div = (div / 2);

		result = std::to_string(remainder) + result;
		count++;
	}

	return result;
}


Uint32 World::fromBinary(std::string num)
{
	Uint32 decResult = 0;

	for (unsigned int i = num.size() - 1; i > 0; i--)
	{
		if (num.at(i) == '1')
		{
			decResult += (int)pow(2, (num.size() - 1) - i);
		}
	}

	return decResult;
}
