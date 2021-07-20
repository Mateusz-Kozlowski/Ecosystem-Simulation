#pragma once

#include "State.h"
#include "Button.h"
#include "InputField.h"

class LoadingState : public State
{
public:
	LoadingState(StateData* state_data);

	// public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

	// mutators:
	virtual void freeze();

private:
	sf::RectangleShape background;

	sf::Font font;

	std::unordered_map<const char*, std::unique_ptr<sf::Text>> texts;

	std::unique_ptr<gui::InputField> inputField;
	std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

	// private methods:
	// initialization:
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initTexts();
	void initInputField();
	void initButtons();

	// other private methods:
	virtual void updateInput();
	void getUpdateFromButtons();
};
