#pragma once

#include "State.h"
#include "Gui.h"

class MainMenuState : public State
{
public:
	// constructor/destructor:
	MainMenuState(StateData* state_data);
	~MainMenuState();

	// mutators:
	void freeze();

	void update(float dt);
	void render(sf::RenderTarget* target = nullptr);

private:
	sf::RectangleShape backgroundRect;
	sf::Font font;
	std::map<std::string, gui::Button*> buttons;
	sf::Text ecosystemText;

	// initialization:
	void initKeybinds();
	void initBackground();
	void initFonts();
	void initButtons();
	void initEcosystemText();

	// other private methods:
	void updateInput();
	void updateButtons();
	void updateEcosystemText();
	void renderButtons(sf::RenderTarget& target);
	void renderEcosystemText(sf::RenderTarget& target);
};
