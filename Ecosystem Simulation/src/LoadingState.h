#pragma once

#include "State.h"
#include "Button.h"
#include "InputField.h"

class LoadingState : public State
{
public:
	// constructor/destructor:
	LoadingState(StateData* state_data);
	~LoadingState();

	// mutators:
	virtual void freeze();

	// other public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

private:
	sf::RectangleShape background;

	sf::Font font;

	std::unordered_map<std::string, gui::Button*> buttons;
	gui::InputField* inputField;

	// initialization:
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initInputField();
	void initButtons();

	// other public methods:
	virtual void updateInput();
	void getUpdateFromButtons();
};
