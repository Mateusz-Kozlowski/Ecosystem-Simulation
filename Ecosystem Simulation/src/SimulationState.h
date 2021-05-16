#pragma once

#include "State.h"
#include "SideMenu.h"

class SimulationState : public State
{
public:
	// constructor/destructor:
	SimulationState(StateData* state_data);
	~SimulationState();

	// mutators:
	virtual void freeze();

	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

private:
	bool paused;
	
	sf::Font font;
	
	sf::View view;

	sf::Vector2i previousMousePosWindow;

	bool sideMenuIsRendered;

	gui::SideMenu* sideMenu;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	// initialization:
	virtual void initKeybinds();
	void initVariables();
	void initFonts();
	void initEcosystem();
	void initView();
	void initDeferredRender();
	void initSideMenu();
	
	// other private methods:
	virtual void updateInput();
	void updateView();
	virtual void updateMousePositions(const sf::View* view = nullptr);
	void getUpdateFromSideMenuGui();
};
