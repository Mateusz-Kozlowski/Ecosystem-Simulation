#pragma once

#include "State.h"
#include "SaveAsPanel.h"
#include "BrainPreviewModifier.h"

class SimulationState : public State
{
public:
	SimulationState(StateData* stateData);
	
	virtual void update(float dt) override;
	virtual void render(sf::RenderTarget* target = nullptr) override;

	// mutators:

	virtual void freeze() override;
	
private:
	// initialization:

	virtual void initKeybinds() override;
	void initVariables();
	void initFonts();
	void initEcosystem();
	void initView();
	void initDeferredRender();
	void initSideMenu();
	void initGodToolsGui();
	void initBrainPreviewModifier();
	void initSaveAsPanel();
	
	// utils:

	virtual void updateInput() override;
	void updateInputWithPanelRendered();

	virtual void updateMousePositions(const sf::View* view = nullptr) override;

	void updateSideMenuVisibility();

	void updateSideMenu();
	void updateSideMenuGui();
	void updateGodToolButton(const std::string& godToolBtnKey);

	void getUpdatesFromSideMenuGui();

	void getUpdatesFromSaveAsPanel();

	void updateView();

	void updateEcosystem(float dt);
	void useEcosystemGodTools();

	void updateBrainPreviewModifier();
	void getUpdateFromAnimalsBrainsPreviews();

private:
	std::unordered_map<std::string, sf::Font> m_fonts;

	sf::View m_view;

	sf::Vector2i m_previousMousePosWindow;

	bool m_sideMenuIsRendered;
	std::unique_ptr<gui::SideMenu> m_sideMenu;

	sf::RenderTexture m_renderTexture;
	sf::Sprite m_renderSprite;

	std::unique_ptr<gui::BrainPreviewModifier> m_brainPreviewModifier;

	bool m_saveAsPanelIsRendered;
	std::unique_ptr<gui::SaveAsPanel> m_saveAsPanel;
};
