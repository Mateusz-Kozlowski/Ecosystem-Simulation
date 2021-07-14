#pragma once

#include "State.h"

class SimulationState : public State
{
public:
	SimulationState(StateData* state_data);
	virtual ~SimulationState();

	// public methods:
	virtual void freeze();
	
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);
	
private:
	bool m_isPaused;

	std::unordered_map<std::string, sf::Font> fonts;

	sf::View view;

	sf::Vector2i previousMousePosWindow;

	bool m_sideMenuIsRendered;

	gui::SideMenu* sideMenu;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	// private methods:

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

	virtual void updateMousePositions(const sf::View* view = nullptr);
	
	void updateSideMenu();
	void updateSideMenuGui();
	void updateGodToolButton(const std::string& god_tool_btn_key);

	void getUpdatesFromSideMenuGui();
	
	void updateView();
	
	void updateEcosystem(float dt);

	void useEcosystemGodTools();
};
