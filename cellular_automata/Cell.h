#pragma once
#include <SDL.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//=============================================================================
// Cell class:
// by William Sullivan
//
// Cell class stores information about itself (ex. age, energy, color).  A cell
// can be one of 3 different types, either plant, animal (herbivore), or dead.
// Cells also know of their neighbors in the 8 possible directions.  Allowing
// them to find potential mates, food, or empty places to reproduce to.
//=============================================================================


class Cell
{
private:
	// Private Member variables
	const int max_age = 200;		//-Max age of a cell before death.

	//-Pointers to cells this cell is aware of.
	//-Index north is zero. Going clockwise.
	Cell* _neighbors[8];

	// -----------Color---------------------
	Uint8 _red;						//-8-bit red value
	Uint8 _green;					//-8-bit green value
	Uint8 _blue;					//-8-bit blue value

	// -----------Attributes----------------
	int _type;						//-0 = Dead, 1 = Plant, 2 = Animal
	int _age;						//-Age of cell
	int _energy;					//-Current energy
	bool _reproduce;				//-Flag, if able to reproduce
	bool _can_act;					//-Flag, if able to act. New cells cannot act.
	enum State { idle, hungry, looking_for_mate };
	enum Type {plant, animal, dead};
	State _current_type;

public:

	// Constructor
	Cell();

	// Initializers
	void birth(int type);
	void birth(int type, Uint32 color);
	void die();

	// Action methods
	void update();
	Cell* findMate(int start_index, bool clockwise);
	Cell* findEmpty(int start_index, bool clockwise);
	Cell* findFood(int start_index, bool clockwise, bool picky);

	// Get methods
	Uint32 myColor();
	int getType() { return _type; }
	int getRed() { return _red; }
	int getGreen() { return _green; }
	int getBlue() { return _blue; }
	int getAge() { return _age; }
	int getEnergy() { return _energy; }
	bool isAlive();
	bool isPlant();
	bool isAnimal();
	bool canReproduce() { return _reproduce; }
	bool canAct();

	// Set methods
	void setNeighbors(Cell* neighborList[]);
	void equals(Cell* cell);
	void setAge(int value) { _age = value; }
	void increaseAge(int amount);
	void setEnergy(int value) { _energy = value; }
	void increaseEnergy(int amount);
	void decreaseEnergy(int amount);
	void make_active() { _can_act = true; }
};