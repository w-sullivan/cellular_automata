#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Screen.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UITextBox.h"

class Options
{
public:
	// Constructors & Destructor
	Options(int x_pos, int y_pos, int width, int height);
	~Options();


	// Option Methods
	bool clear();
	static int getAgeDeath() { return _age_death_possible; }
	static int getAnimalPref() { return _picky_animal; }
	static int getCreationType() { return _creation_type; }
	static int getGrowthRate() { return _growth_rate; }
	static int getMutationRate() { return _mutation_rate; }
	bool paused() { return _is_paused; }
	bool quit() { return _should_quit; }
	void setCreationType(int type);
	void setGrowthRate(int value);
	bool toggleAgeDeath();
	bool toggleAnimalPreference();
	void togglePause();

	// UI Methods
	void addToTextbox(char character);
	bool clickAt(int x, int y);
	void draw();
	bool editingText() { return _editing_text; }
	TTF_Font* getFont() { return _font; }
	void stopEditingText();
	void textboxBackspace();

private:
	// Option variables
	bool _is_paused;
	bool _should_quit;
	bool _should_clear;
	bool _editing_text;
	int _food_value = 1;					//-How much energy is food worth
	static int _mutation_rate;				//-Chance of mutation. 1 out of x
	static int _creation_type;
	static int _growth_rate;
	static bool _age_death_possible;
	static bool _picky_animal;

	// UI variables
	int _textbox_index;
	int _textbox_count;
	std::string _temp_text;
	TTF_Font* _font;
	SDL_Rect _rect;
	SDL_Color _label_text_color;
	SDL_Color _button_text_color;
	SDL_Texture* _background;

	// UI components
	UILabel* _click_type_label;
	UILabel* _mutation_rate_label;
	UILabel* _plant_growth_rate_label;
	UIButton* _plant_button;
	UIButton* _animal_button;
	UIButton* _delete_button;
	UIButton* _pause_button;
	UIButton* _clear_button;
	UIButton* _exit_button;
	UICheckBox* _plant_death_checkbox;
	UICheckBox* _animal_preference_checkbox;
	UITextBox* _textboxes[2];

	// Private Methods
	bool createBackground();
	bool inOptions(int x, int y);
};