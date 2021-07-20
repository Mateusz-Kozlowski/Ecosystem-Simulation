#pragma once

#include "State.h"
#include "SaveAsPanel.h"

class SimulationState : public State
{
public:
	SimulationState(StateData* state_data);

	// public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

	// mutators:
	virtual void freeze();
	
private:
	std::unordered_map<std::string, sf::Font> fonts;

	sf::View view;

	sf::Vector2i previousMousePosWindow;

	bool m_sideMenuIsRendered;

	std::unique_ptr<gui::SideMenu> sideMenu;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	bool m_saveAsPanelIsRendered;

	std::unique_ptr<gui::SaveAsPanel> saveAsPanel;

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
	void initSaveAsPanel();

	// other private methods:
	virtual void updateInput();
	void updateInputWithPanelRendered();

	virtual void updateMousePositions(const sf::View* view = nullptr);
	
	void updateSideMenu();
	void updateSideMenuGui();
	void updateGodToolButton(const std::string& god_tool_btn_key);

	void getUpdatesFromSideMenuGui();

	void getUpdatesFromSaveAsPanel();
	
	void updateView();
	
	void updateEcosystem(float dt);

	void useEcosystemGodTools();
};
