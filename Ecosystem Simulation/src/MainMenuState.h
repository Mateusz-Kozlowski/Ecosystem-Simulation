#pragma once

#include "SimulationState.h"
#include "EcosystemCreatorState.h"
#include "LoadingState.h"
#include "Gui.h"

class MainMenuState : public State
{
public:
	MainMenuState(StateData* stateData);
	
	virtual void update(float dt) override;
	virtual void render(sf::RenderTarget* target = nullptr) override;

	// mutators:

	virtual void freeze() override;

private:	
	void initVariables();
	virtual void initKeybinds() override;
	void initBackground();
	void initFonts();
	void initButtons();
	void initEcosystemText();

	void highlightEcosystemText();
	void saveEcosystem(const Ecosystem& ecosystem);

	virtual void updateInput() override;
	void getUpdateFromButtons();
	void updateEcosystemText(float dt);

	void renderButtons(sf::RenderTarget& target);

private:
	typedef std::unique_ptr<gui::Button> BtnUniquePtr;

	sf::RectangleShape m_backgroundRect;
	
	std::unordered_map<std::string, sf::Font> m_fonts;

	std::unordered_map<std::string, BtnUniquePtr> m_buttons;

	sf::Text m_ecosystemName;

	sf::Color m_defaultColorOfEcosystemName;
	sf::Color m_highlightedColorOfEcosystemName;
	
	float m_ecosystemTextStopwatch;
	float m_highlightningTime;
};
