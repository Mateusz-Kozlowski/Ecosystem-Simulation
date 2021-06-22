#pragma once

#include "SimulationState.h"
#include "LoadingState.h"
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
	
	std::unordered_map<std::string, sf::Font> fonts;

	std::unordered_map<std::string, gui::Button*> buttons;

	sf::Text ecosystemText;

	sf::Color defaultEcosystemTextColor;
	sf::Color highlightedEcosystemTextColor;
	
	float ecosystemTextStopwatch;
	float highlightningTime;

	// initialization:
	void initVariables();
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initButtons();
	void initEcosystemText();

	// private utilities:
	void highlightEcosystemText();
	void saveEcosystem(const Ecosystem& ecosystem);

	// other private methods:
	virtual void updateInput();
	void getUpdateFromButtons();
	void updateEcosystemText(float dt);
	
	void renderButtons(sf::RenderTarget& target);
	void renderEcosystemText(sf::RenderTarget& target);
};
