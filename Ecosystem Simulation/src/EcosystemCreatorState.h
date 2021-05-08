#pragma once

#include "State.h"
#include "Button.h"
#include "InputField.h"

class EcosystemCreatorState : public State
{
public:
	// constructor/destructor:
	EcosystemCreatorState(StateData* state_data);
	virtual ~EcosystemCreatorState();

	// mutators:
	virtual void freeze();

	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

private:
	sf::Font font;
	
	std::map<std::string, gui::Button*> buttons;

	gui::InputField* inputField;

	// initialization:
	virtual void initKeybinds();
	void initFonts();
	void initButtons();
	void initInputField();

	// other private methods:
	virtual void updateInput();
	void updateButtons();
	void renderButtons(sf::RenderTarget& target);
};
