#pragma once

#include "State.h"
#include "PauseMenu.h"

class SimulationState : public State
{
public:
	// constructor/destructor:
	SimulationState(StateData* state_data);
	~SimulationState();

	// mutators:
	void freeze();

	void update(float dt);
	void render(sf::RenderTarget* target = nullptr);

private:
	sf::Font font;
	
	sf::View view;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	gui::PauseMenu* pauseMenu;

	bool paused;

	// initialization:
	void initKeybinds();
	void initVariables();
	void initFonts();
	void initEcosystem();
	void initView();
	void initDeferredRender();
	void initPauseMenu();
	
	// other private methods:
	void updateInput();
	void updateView();
	void updatePauseMenuButtons();
};
