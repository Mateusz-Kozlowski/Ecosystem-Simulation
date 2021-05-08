#pragma once

#include "SimulationState.h"
#include "EcosystemCreatorState.h"
#include "Gui.h"

class MainMenuState : public State
{
public:
	// constructor/destructor:
	MainMenuState(StateData* state_data);
	virtual ~MainMenuState();

	// mutators:
	virtual void freeze();

	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

private:
	sf::RectangleShape backgroundRect;
	sf::Font font;
	sf::Font font2;
	std::map<std::string, gui::Button*> buttons;
	sf::Text ecosystemText;

	// initialization:
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initButtons();
	void initEcosystemText();

	// other private methods:
	virtual void updateInput();
	void updateButtons();
	void updateEcosystemText();
	void renderButtons(sf::RenderTarget& target);
	void renderEcosystemText(sf::RenderTarget& target);
};
