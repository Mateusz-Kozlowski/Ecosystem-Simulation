#pragma once

#include "State.h"
#include "SideMenu.h"

class SimulationState : public State
{
public:
	// constructor/destructor:
	SimulationState(StateData* state_data);
	virtual ~SimulationState();

	// mutators:
	virtual void freeze();

	// other public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

private:
	bool paused;

	std::unordered_map<std::string, sf::Font> fonts;

	sf::View view;

	sf::Vector2i previousMousePosWindow;

	bool sideMenuIsRendered;

	gui::SideMenu* sideMenu;

	std::string currentTool;

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
	void initGodToolsGui();

	// other private methods:
	virtual void updateInput();
	void updateView();
	virtual void updateMousePositions(const sf::View* view = nullptr);
	void updateSideMenu();
	void getUpdatesFromSideMenuGui();
	void updateEcosystem(float dt);

	// private utilities:
	void updateSideMenuGui();
	void updateGodToolButton(const std::string& god_tool_btn_key);
};
