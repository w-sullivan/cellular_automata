#include "Cell.h"

//===============================CONSTRUCTORS==================================

// Default constructor
Cell::Cell()
{
	_age = 0;
	_red = 0;
	_green = 0;
	_blue = 0;
	_energy = 0;
	_reproduce = false;
	_type = 0;
	_can_act = false;

	for (int i = 0; i < 8; i++)
	{
		_neighbors[i] = nullptr;
	}

	// Initialize random seed
	srand(time(NULL));
}



//===============================INITIALIZERS==================================

// Initializes a default living cell, with given type.
void Cell::birth(int type)
{
	die();

	// If plant
	if (type == 1)
	{
		_red = 0;
		_green = 255;
		_energy = 0;
	}
	// If Animal
	else if (type == 2)
	{
		_red = 100;
		_green = 100; 
		_energy = 50;
	}
	else
	{
		std::cout << "Trying to birth an unknown type.  Creating a dead cell." << std::endl;
		_red = 0;
		_green = 0;
	}

	_age = 0;
	_blue = 0;
	_reproduce = false;
	_type = type;
	_can_act = false;
}


// Initializes a living cell with a type and a given 32-bit decimal color
void Cell::birth(int type, Uint32 color)
{
	die();
	_age = 0;
	_energy = 0;
	_reproduce = false;
	_type = type;
	_can_act = false;

	// Set color values.
	color >>= 8;
	_blue = color & 0x000000FF;
	color >>= 8;
	_green = color & 0x000000FF;
	color >>= 8;
	_red = color & 0x000000FF;
}


// Initializes a dead cell
void Cell::die()
{
	_age = 0;
	_red = 0;
	_green = 0;
	_blue = 0;
	_energy = 0;
	_reproduce = false;
	_type = 0;
	_can_act = false;
}



//=============================ACTION METHODS==================================

void Cell::update()
{
	if (_current_type == plant)
	{
		increaseAge(1);
	}
	else if (_current_type == animal)
	{
		increaseAge(1);
	}
	else  // dead
	{
		return;
	}
}


// Checks neighbor cells to see if there are any around that can reproduce
// * start_index: the direction to start looking
// * clockwise: whether to look clockwise or anti-clockwise
Cell* Cell::findMate(int start_index, bool clockwise)
{
	int index = start_index;

	do
	{
		// If a neighbor exists, AND is same type, AND can reproduce
		if (_neighbors[index] != nullptr && _type == _neighbors[index]->getType() && _neighbors[index]->canReproduce())
		{
			return _neighbors[index];
		}

		// Keep looking clockwise
		if (clockwise)
		{
			index++;
			if (index > 7)
			{
				index = 0;
			}
		}
		else // counter-clockwise
		{
			index--;
			if (index < 0)
			{
				index = 7;
			}
		}
	} while (index != start_index);

	// Nothing found
	return nullptr;
}


// Returns the location of an empty neighbor
// * start_index: the direction to start looking
// * clockwise: whether to look clockwise or anti-clockwise
Cell* Cell::findEmpty(int start_index, bool clockwise)
{
	int index = start_index;

	do
	{
		// If a neighbor is found to be empty, return a pointer to it.
		if (_neighbors[index] != nullptr && _neighbors[index]->isAlive() == false)
		{
			return _neighbors[index];
		}

		// Keep looking clockwise
		if (clockwise)
		{
			index++;
			if (index > 7)
			{
				index = 0;
			}
		}
		else // counter-clockwise
		{
			index--;
			if (index < 0)
			{
				index = 7;
			}
		}
	} while (index != start_index);

	// Nothing found
	return nullptr;
}


// Returns the location of a food (plant) neighbor
// * start_index: the direction to start looking
// * clockwise: whether to look clockwise or anti-clockwise
Cell* Cell::findFood(int start_index, bool clockwise, bool picky)
{
	int index = start_index;
	float highest_value = 0;
	int highest_index = start_index;

	// Current vision distance is one cell. (May increase this for 'smarter' animals)
	do
	{
		// If a neighbor is alive and a plant.
		if (_neighbors[index] != nullptr && _neighbors[index]->isAlive() == true && _neighbors[index]->isPlant())
		{
			// If animals prefer like color plants
			if (picky)
			{
				// Check nutrients level.
				// Calculate the difference between red values
				float red_level = (float)_neighbors[index]->getRed();
				if (red_level == 0 || _red == 0)
					red_level = 0;
				else if (red_level >= _red)
					red_level = _red / red_level;
				else
					red_level = red_level / _red;

				// Calculate the difference between green values
				float green_level = (float)_neighbors[index]->getGreen();
				if (green_level == 0 || _green == 0)
					green_level = 0;
				else if (green_level >= _green)
					green_level = _green / green_level;
				else
					green_level = green_level / _green;

				// Calculate the difference between blue values
				float blue_level = (float)_neighbors[index]->getBlue();
				if (blue_level == 0 || _blue == 0)
					blue_level = 0;
				else if (blue_level >= _blue)
					blue_level = _blue/ blue_level;
				else
					blue_level = blue_level / _blue;


				// Get the average
				float average_level = (red_level + green_level + blue_level) / 3;

				float food_energy = average_level * 10;

				if (food_energy > highest_value)
				{
					highest_value = food_energy;
					highest_index = index;
				}
			}
			else
			{
				// Food found. Eat it and tell world where you found it. 
				// Eat, gain 5 energy
				increaseEnergy(5);
				return _neighbors[index];
			}
		}

		// Increment Index
		// Keep looking clockwise
		if (clockwise)
		{
			index++;
			if (index > 7)
			{
				index = 0;
			}
		}
		else // counter-clockwise
		{
			index--;
			if (index < 0)
			{
				index = 7;
			}
		}
	} while (index != start_index);

	if (picky)
	{
		// eat the cell with the highest nutrition value.
		increaseEnergy(int(round(highest_value)));
		return _neighbors[highest_index];
	}

	// Nothing found
	return nullptr;
}



//===============================GET METHODS===================================

// Returns the 32-bit decimal color from the RGB values
Uint32 Cell::myColor()
{
	Uint32 color = 0;

	color += _red;
	color <<= 8;
	color += _green;
	color <<= 8;
	color += _blue;
	color <<= 8;
	color += 0xFF;

	return color;
}


// Returns true if cell type is dead
bool Cell::isAlive()
{
	if (_type != 0)
		return true;
	return false;
}


// Returns true if cell type is plant
bool Cell::isPlant()
{
	if (_type == 1)
		return true;
	return false;
}


// Returns true if cell type is plant
bool Cell::isAnimal()
{
	if (_type == 2)
		return true;
	return false;
}


// Returns true if the cell can act
bool Cell::canAct()
{
	if (_can_act)
		return true;
	return false;
}



//===============================SET METHODS===================================

// Sets the neighbors for a cell from a given pointer list
void Cell::setNeighbors(Cell* neighborList[])
{
	for (int i = 0; i < 8; i++)
	{
		_neighbors[i] = neighborList[i];
	}
}


// Sets the cells values equal to the passed in cell.
//  Used to move.  Sets _can_act to false.
void Cell::equals(Cell* cell)
{
	_red = cell->getRed();
	_green = cell->getGreen();
	_blue = cell->getBlue();
	_type = cell->getType();
	_age = cell->getAge();
	_energy = cell->getEnergy();
	_reproduce = cell->canReproduce();
	_can_act = false;
}


// Increase the age of a cell by amount
// If cell's max age is reached, the cell dies.
void Cell::increaseAge(int amount)
{
	_age += amount;

	if (pow(rand() % max_age, 4) <= pow(_age, 4))
	//if(_age == max_age)
		die();
}


// Increases an cells energy by an amount.
// If the energy reaches 100, the cell can reproduce.
void Cell::increaseEnergy(int amount)
{
	_energy += amount;
	if (_energy >= 60)
	{
		_reproduce = true;
	}
	if (_energy >= 100)
	{
		_energy = 100;
	}
}


// Decreases a cells energy by an amount. If energy reaches
// zero, the cell dies
void Cell::decreaseEnergy(int amount)
{
	_energy -= amount;
	
	if (_energy < 60)
	{
		_reproduce = false;
	}
	if (_energy <= 0)
	{
		_energy = 0;
		die();
	}
}
