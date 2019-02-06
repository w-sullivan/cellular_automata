#include "Options.h"

int Options::_mutation_rate = 1000;
int Options::_creation_type = 1;
int Options::_growth_rate = 10;
bool Options::_age_death_possible = false;
bool Options::_picky_animal = false;

//========================= Construtor & Destructor============================
Options::Options(int x_pos, int y_pos, int width, int height)
{
	_rect.x = x_pos;
	_rect.y = y_pos;
	_rect.w = width;
	_rect.h = height;

	_is_paused = false;
	_should_quit = false;
	_should_clear = false;
	_temp_text = "";

	SDL_Color white;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;

	SDL_Color black;
	black.r = 0;
	black.g = 0;
	black.b = 0;
	black.a = 255;

	_label_text_color = white;
	_button_text_color = black;

	// Initialize True Type Font
	if (TTF_Init() != -1)
	{
		std::cout << "True type font initialized..." << std::endl;
	}
	else
	{
		std::cerr << "True type font failed to initialize!" << std::endl;
	}

	_font = TTF_OpenFont("font/coure.fon", 14);

	createBackground();

	// UI components
	// Labels
	_click_type_label = new UILabel(_rect.x + 10, 15, "Create on click:", _font, _label_text_color);
	_mutation_rate_label = new UILabel(_rect.x + 20, 140, "Mutation Rate: (1/x)", _font, _label_text_color);
	_plant_growth_rate_label = new UILabel(_rect.x + 20, 200, "Plant Growth rate: (1-100)", _font, _label_text_color);

	// Buttons
	_plant_button = new UIButton(_rect.x + 10, 35, 75, 40, "Plant", _font, _button_text_color);
	_animal_button = new UIButton(_rect.x + 90, 35, 75, 40, "Animal", _font, _button_text_color);
	_delete_button = new UIButton(_rect.x + 170, 35, 75, 40, "Delete", _font, _button_text_color);
	_pause_button = new UIButton(_rect.x + 30, _rect.h - 140, 75, 40, "Pause", _font, _button_text_color);
	_clear_button = new UIButton(_rect.x + 30, _rect.h - 95, 200, 40, "Clear", _font, _button_text_color);
	_exit_button = new UIButton(_rect.x + 30, _rect.h - 50, 200, 40, "Exit", _font, _button_text_color);

	// Checkboxes
	_plant_death_checkbox = new UICheckBox(_rect.x + 20, 100, "Plants die to old age", _font, _label_text_color);
	_animal_preference_checkbox = new UICheckBox(_rect.x + 20, 250, "Animals prefer like colored plants", _font, _label_text_color);

	// Textboxes
	// Mutation Rate
	_textboxes[0] = new UITextBox(_rect.x + 20, 155, 200, 30, std::to_string(_mutation_rate), _font, _label_text_color);
	// Energy increase rate
	_textboxes[1] = new UITextBox(_rect.x + 20, 215, 200, 30, std::to_string(_growth_rate), _font, _label_text_color);

	_textbox_count = 2;
	_textbox_index = 0;
	_editing_text = false;

	// Set default state
	_plant_button->setEnabled(true);
}


Options::~Options()
{
	delete _click_type_label;
	delete _mutation_rate_label;
	delete _plant_growth_rate_label;
	delete _plant_button;
	delete _animal_button;
	delete _delete_button;
	delete _pause_button;
	delete _clear_button;
	delete _exit_button;
	delete _plant_death_checkbox;
	delete _animal_preference_checkbox;

	for(int i = 0; i < _textbox_count; i++)
		delete _textboxes[i];

	TTF_CloseFont(_font);
	TTF_Quit();
}


//============================== Option Methods ===============================

bool Options::clear()
{
	if (_should_clear)
	{
		_should_clear = false;
		return true;
	}

	return false;
}


void Options::setCreationType(int type)
{
	if (type >= 0 && type <= 2)
	{
		_creation_type = type;
	}
}


void Options::setGrowthRate(int rate)
{
	// Divide the rate by 2.  Since anything over 50 causes plants to
	// reproduce at the same rate.

	if (rate <= 0)
	{
		_growth_rate = 0;
		return;
	}

	if (rate % 2 == 1)
	{
		_growth_rate = (rate / 2) + 1;
	}
	else
		_growth_rate = rate / 2;
}


bool Options::toggleAgeDeath()
{
	if (_age_death_possible)
	{
		_age_death_possible = false;
		return _age_death_possible;
	}
	else
	{
		_age_death_possible = true;
		return _age_death_possible;
	}
}


bool Options::toggleAnimalPreference()
{
	if (_picky_animal)
		_picky_animal = false;
	else
		_picky_animal = true;

	return _picky_animal;
}


void Options::togglePause()
{
	if (_is_paused)
	{
		_is_paused = false;
	}
	else
	{
		_is_paused = true;
	}
}


//================================ UI Methods =================================

void Options::addToTextbox(char character)
{
	if ('0' <= character && character <= '9')
	{
		_temp_text += character;
		_textboxes[_textbox_index]->setLabelText(_temp_text);
	}
}


bool Options::clickAt(int x, int y)
{
	if (!inOptions(x, y))
		return false;

	// Plant button
	if (_plant_button->clickedOn(x, y))
	{
		_plant_button->setEnabled(true);
		_animal_button->setEnabled(false);
		_delete_button->setEnabled(false);
		_creation_type = 1;
	}
	else if (_animal_button->clickedOn(x, y))
	{
		_plant_button->setEnabled(false);
		_animal_button->setEnabled(true);
		_delete_button->setEnabled(false);
		_creation_type = 2;
	}
	else if (_delete_button->clickedOn(x, y))
	{
		_plant_button->setEnabled(false);
		_animal_button->setEnabled(false);
		_delete_button->setEnabled(true);
		_creation_type = 0;
	}
	else if (_pause_button->clickedOn(x, y))
	{
		togglePause();
		_pause_button->toggleEnabled();
	}
	else if (_clear_button->clickedOn(x, y))
	{
		_should_clear = true;
	}
	else if (_exit_button->clickedOn(x, y))
	{
		_should_quit = true;
	}
	else if (_plant_death_checkbox->clickedOn(x, y))
	{
		_plant_death_checkbox->toggleEnabled();
		toggleAgeDeath();
	}
	else if (_animal_preference_checkbox->clickedOn(x, y))
	{
		_animal_preference_checkbox->toggleEnabled();
		toggleAnimalPreference();
	}

	// textboxes
	for (int i = 0; i < _textbox_count; i++)
	{
		if (_textboxes[i]->clickedOn(x, y))
		{
			_textbox_index = i;
			_temp_text = "";
			_textboxes[i]->toggleEnabled();
			_textboxes[i]->setLabelText(_temp_text);
			_editing_text = true;
		}
	}

	return true;
}


void Options::draw()
{
	// Draw background
	SDL_RenderCopy(Screen::renderer, _background, NULL, &_rect);

	// Draw labels
	_click_type_label->draw();
	_mutation_rate_label->draw();
	_plant_growth_rate_label->draw();

	// Draw buttons
	_plant_button->draw();
	_animal_button->draw();
	_delete_button->draw();
	_pause_button->draw();
	_clear_button->draw();
	_exit_button->draw();

	// Draw checkboxes
	_plant_death_checkbox->draw();
	_animal_preference_checkbox->draw();

	// Draw TextBoxes
	for (int i = 0; i < _textbox_count; i++)
		_textboxes[i]->draw();

}


void Options::stopEditingText()
{
	_editing_text = false;
	if (_temp_text == "")
	{
		_temp_text = "0";
		_textboxes[_textbox_index]->setLabelText(std::to_string(0));
	}

	// Try to convert string to an integer.
	int num;
	try
	{
		num = std::stoi(_temp_text);
	}
	catch (...)
	{
		num = 2147483647;
		_textboxes[_textbox_index]->setLabelText(std::to_string(num));
	}

	// Pass the integer to the appropriate option variable
	switch (_textbox_index)
	{
		// Mutation Rate textbox
	case 0:
		if (num == 0)
		{
			num = 1;
			_textboxes[_textbox_index]->setLabelText(std::to_string(num));
		}
		_mutation_rate = num;
		break;
		// Growth Rate textbox
	case 1:
		if (num > 100)
		{
			num = 100;
			_textboxes[_textbox_index]->setLabelText(std::to_string(num));
		}
		_growth_rate = num;
		break;
	default:
		std::cerr << "Error: textbox index out of range" << std::endl;
	}

	_textboxes[_textbox_index]->toggleEnabled();
	_temp_text = "";
}


void Options::textboxBackspace()
{
	if (_temp_text.length() > 0)
	{
		_temp_text.pop_back();
		_textboxes[_textbox_index]->setLabelText(_temp_text);
	}
}


//==============================Private Methods ===============================

bool Options::createBackground()
{
	// Background
	_background = SDL_CreateTexture(Screen::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, _rect.w, _rect.h);
	if (_background)
	{
		std::cout << "Creating Options Background..." << std::endl;
	}
	else
	{
		std::cerr << "Options background could not be initialized." << std::endl;
		return false;
	}


	Uint32* buffer = new Uint32[_rect.w * _rect.h];
	int index = 0;

	Uint32 black = 0x000000FF;
	Uint32 white = 0xFFFFFFFF;

	for (int y = 0; y < _rect.h; y++)
	{
		for (int x = 0; x < _rect.w; x++)
		{
			if (y <= 5 || y > _rect.h - 5)
			{
				buffer[index] = white;
			}
			else if (x <= 5 || x > _rect.w - 5)
			{
				buffer[index] = white;
			}
			else
			{
				buffer[index] = black;
			}
			index++;
		}
	}

	SDL_UpdateTexture(_background, NULL, buffer, _rect.w * sizeof(Uint32));

	delete[] buffer;
	return true;
}


bool Options::inOptions(int x, int y)
{
	if (x >= _rect.x && x < _rect.x + _rect.w)
	{
		if (y >= _rect.y && y < _rect.y + _rect.h)
		{
			return true;
		}
	}
	return false;
}
