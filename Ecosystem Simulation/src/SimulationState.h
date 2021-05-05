#pragma once

#include "State.h"
#include "MiniMap.h"
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

	MiniMap* miniMap;

	PauseMenu* pmenu;

	// initialization:
	void initKeybinds();
	void initVariables();
	void initFonts();
	void initEcosystem();
	void initView();
	void initDeferredRender();
	void initMiniMap();
	void initPauseMenu();
	
	// other private methods:
	void updateInput();
	void updateEcosystem();
	void updateView();
	void updatePauseMenu();
	void updateMiniMap();

	void renderEcosystem(sf::RenderTarget& target);
	void renderMiniMap(sf::RenderTarget& target);
	void renderPauseMenu(sf::RenderTarget& target);
};
