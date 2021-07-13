#pragma once

#include "SimulationState.h"
#include "EcosystemCreatorState.h"
#include "LoadingState.h"
#include "Gui.h"

class MainMenuState : public State
{
public:
	MainMenuState(StateData* state_data);

	// public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

	// mutators:
	virtual void freeze();	

private:
	sf::RectangleShape backgroundRect;
	
	std::unordered_map<std::string, sf::Font> fonts;

	std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

	sf::Text ecosystemText;

	sf::Color defaultEcosystemTextColor;
	sf::Color highlightedEcosystemTextColor;
	
	float ecosystemTextStopwatch;
	float highlightningTime;

	// private methods:
	// initialization:
	void initVariables();
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initButtons();
	void initEcosystemText();

	// other private methods:
	void highlightEcosystemText();
	void saveEcosystem(const Ecosystem& ecosystem);
	
	virtual void updateInput();
	void getUpdateFromButtons();
	void updateEcosystemText(float dt);
	
	void renderButtons(sf::RenderTarget& target);
};
