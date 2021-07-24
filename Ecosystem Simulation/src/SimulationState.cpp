#include "stdafx.h"
#include "SimulationState.h"

SimulationState::SimulationState(StateData* state_data)
	: State(state_data)
{
	this->initKeybinds();
	this->initVariables();
	this->initFonts();
	this->initEcosystem();
	this->initView();
	this->initDeferredRender();
	this->initSideMenu();
	this->initSaveAsPanel();
}

// public methods:
void SimulationState::update(float dt)
{
	if (!this->m_saveAsPanelIsRendered)
		this->updateInput();
	else
		this->updateInputWithPanelRendered();

	this->updateMousePositions(&this->view);

	if (this->m_sideMenuIsRendered)
		this->updateSideMenu();

	this->getUpdatesFromSideMenuGui();

	if (this->m_saveAsPanelIsRendered)
		this->saveAsPanel->update(dt, *this->stateData->events, this->mousePosWindow);

	this->getUpdatesFromSaveAsPanel();

	this->updateView();

	this->updateEcosystem(dt);
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->stateData->window;

	this->renderTexture.clear();

	// draw ecosystem:
	this->renderTexture.setView(this->view);

	this->stateData->ecosystem->render(this->renderTexture);

	// render side menu and save as panel:
	this->renderTexture.setView(this->renderTexture.getDefaultView());

	if (this->m_sideMenuIsRendered)
		this->sideMenu->render(this->renderTexture);

	if (this->m_saveAsPanelIsRendered)
		this->saveAsPanel->render(this->renderTexture);

	// final render:
	this->renderTexture.display();

	target->draw(this->renderSprite);
}

// mutators:
void SimulationState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

// initialization:
void SimulationState::initKeybinds()
{
	const char* path = "config/simulation_keybinds.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key;
		std::string key2;

		while (ifs >> key >> key2)
			this->keybinds[key] = this->stateData->supportedKeys->at(key2);
	}
	else throw("ERROR::SIMULATIONSTATE::COULD NOT OPEN: " + std::string(path) + '\n');

	ifs.close();
}

void SimulationState::initVariables()
{
	this->sideMenu = nullptr;
	this->m_sideMenuIsRendered = false;
	this->m_saveAsPanelIsRendered = false;
	this->previousMousePosWindow = sf::Vector2i(0, 0);
}

void SimulationState::initFonts()
{
	if (!this->fonts["Retroica"].loadFromFile("resources/fonts/Retroica.ttf"))
		throw("ERROR::SIMULATIONSTATE::COULD NOT LOAD A FONT");

	if (!this->fonts["CONSOLAB"].loadFromFile("resources/fonts/CONSOLAB.ttf"))
		throw("ERROR::SIMULATIONSTATE::COULD NOT LOAD A FONT");
}

void SimulationState::initEcosystem()
{
	this->stateData->ecosystem->pauseSimulation();
}

void SimulationState::initView()
{
	this->view.setSize(this->stateData->ecosystem->getWorldSize());

	this->view.setCenter(
		sf::Vector2f(
			this->stateData->ecosystem->getWorldSize().x / 2.f,
			this->stateData->ecosystem->getWorldSize().y / 2.f
		)
	);
}

void SimulationState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
	);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(
		sf::IntRect(
			0,
			0,
			this->stateData->gfxSettings->resolution.width,
			this->stateData->gfxSettings->resolution.height
		)
	);
}

void SimulationState::initSideMenu()
{
	// temporary variables:
	const std::string guiPath = "resources/textures/GUI";

	const sf::VideoMode resolution = this->stateData->gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(24.0f, resolution);

	// create new SideMenu:
	this->sideMenu = std::make_unique<gui::SideMenu>(
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(gui::p2pX(24.0f, resolution), gui::p2pY(100.0f, resolution)),
		sf::Color(48, 48, 48)
	);

	// add widgets:
	this->sideMenu->addCenteredText(
		gui::p2pY(4.0f, resolution),
		charSize,
		this->fonts["CONSOLAB"],
		"PLAY/STOP:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addImageButton(
		"PAUSE",
		{
			{"PLAY IDLE", guiPath + "/play and stop/play.png"},
			{"STOP IDLE", guiPath + "/play and stop/stop.png"},

			{"PLAY HOVERED", guiPath + "/play and stop/play light.png"},
			{"STOP HOVERED", guiPath + "/play and stop/stop light.png"},

			{"PLAY PRESSED", guiPath + "/play and stop/play dark.png"},
			{"STOP PRESSED", guiPath + "/play and stop/stop dark.png"}
		},
		"PLAY IDLE",
		sf::Vector2f(
			gui::p2pX(10.33f, resolution),
			gui::p2pY(8.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);
	
	this->sideMenu->addCenteredText(
		gui::p2pY(17.5f, resolution),
		charSize,
		this->fonts["CONSOLAB"],
		"SPEED:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addSlider(
		"SPEED",
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(23.0f, resolution)
		),
		256.0f / 1840.0f,
		{ 0.0f, 1.0f },
		{ 0.01f, 0.81f },
		1.0f,
		guiPath + "/scale sliders/axes/axis.png",
		guiPath + "/scale sliders/handles/handle.png",

		guiPath + "/scale sliders/axes/axis light.png",
		guiPath + "/scale sliders/handles/handle light.png",

		guiPath + "/scale sliders/axes/axis dark.png",
		guiPath + "/scale sliders/handles/handle dark.png",
		"quadratic"
	);

	this->sideMenu->addCenteredText(
		gui::p2pY(28.5f, resolution),
		charSize,
		this->fonts["CONSOLAB"],
		"MOVE THIS PANEL:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addImageButton(
		"ARROW",
		{
			{"LEFT IDLE", guiPath + "/arrows/left arrow.png"},
			{"RIGHT IDLE", guiPath + "/arrows/right arrow.png"},

			{"LEFT HOVERED", guiPath + "/arrows/left arrow light.png"},
			{"RIGHT HOVERED", guiPath + "/arrows/right arrow light.png"},

			{"LEFT PRESSED", guiPath + "/arrows/left arrow dark.png"},
			{"RIGHT PRESSED", guiPath + "/arrows/right arrow dark.png"}
		},
		"RIGHT IDLE",
		sf::Vector2f(
			gui::p2pX(10.33f, resolution),
			gui::p2pY(32.5f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	this->initGodToolsGui();

	this->sideMenu->addCenteredText(
		gui::p2pY(61.0f, resolution),
		charSize,
		this->fonts["CONSOLAB"],
		"ZOOM:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addImageButton(
		"ZOOM IN",
		{
			{"IDLE", guiPath + "/zoom/zoom in.png"},
			{"HOVERED", guiPath + "/zoom/zoom in light.png"},
			{"PRESSED", guiPath + "/zoom/zoom in dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(8.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(65.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	this->sideMenu->addImageButton(
		"ZOOM OUT",
		{
			{"IDLE", guiPath + "/zoom/zoom out.png"},
			{"HOVERED", guiPath + "/zoom/zoom out light.png"},
			{"PRESSED", guiPath + "/zoom/zoom out dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(12.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(65.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	this->sideMenu->addButton(
		"SAVE",
		sf::Vector2f(
			gui::p2pX(6.0f, resolution),
			gui::p2pY(75.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		charSize,
		this->fonts["CONSOLAB"],
		"SAVE",
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->sideMenu->addButton(
		"SAVE AS",
		sf::Vector2f(
			gui::p2pX(6.f, resolution),
			gui::p2pY(83.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		charSize,
		this->fonts["CONSOLAB"],
		"SAVE AS",
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->sideMenu->addButton(
		"QUIT",
		sf::Vector2f(
			gui::p2pX(6.f, resolution),
			gui::p2pY(91.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		charSize,
		this->fonts["CONSOLAB"],
		"QUIT",
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);
}

void SimulationState::initGodToolsGui()
{
	// temporary variables:
	const std::string& guiPath = "resources/textures/GUI";

	const sf::VideoMode resolution = this->stateData->gfxSettings->resolution;

	float posYpercentage = 45.5f;

	// init God tools GUI:
	this->sideMenu->addCenteredText(
		gui::p2pY(41.5f, resolution),
		gui::calcCharSize(24.0f, resolution),
		this->fonts["CONSOLAB"],
		"GOD TOOLS:",
		sf::Color(225, 225, 225)
	);

	this->sideMenu->addImageButton(
		"TRACK",
		{
			{"IDLE", guiPath + "/God tools/track/track.png"},
			{"LIGHT", guiPath + "/God tools/track/track light.png"},
			{"DARK", guiPath + "/God tools/track/track dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(6.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(posYpercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	this->sideMenu->addImageButton(
		"KILL",
		{
			{"IDLE", guiPath + "/God tools/kill/kill.png"},
			{"LIGHT", guiPath + "/God tools/kill/kill light.png"},
			{"DARK", guiPath + "/God tools/kill/kill dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(10.f + 1.f / 3.f, resolution),
			gui::p2pY(posYpercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addImageButton(
		"REPLACE",
		{
			{"IDLE", guiPath + "/God tools/replace/replace.png"},
			{"LIGHT", guiPath + "/God tools/replace/replace light.png"},
			{"DARK", guiPath + "/God tools/replace/replace dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(14.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(posYpercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addImageButton(
		"BRAIN",
		{
			{"IDLE", guiPath + "/God tools/brain/brain.png"},
			{"LIGHT", guiPath + "/God tools/brain/brain light.png"},
			{"DARK", guiPath + "/God tools/brain/brain dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(6.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(posYpercentage, resolution) + gui::p2pX(4.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	this->sideMenu->addImageButton(
		"STOP",
		{
			{"IDLE", guiPath + "/God tools/stop/stop.png"},
			{"LIGHT", guiPath + "/God tools/stop/stop light.png"},
			{"DARK", guiPath + "/God tools/stop/stop dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(10.f + 1.f / 3.f, resolution),
			gui::p2pY(posYpercentage, resolution) + gui::p2pX(4.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	this->sideMenu->addImageButton(
		"INFO",
		{
			{"IDLE", guiPath + "/God tools/info/info.png"},
			{"LIGHT", guiPath + "/God tools/info/info light.png"},
			{"DARK", guiPath + "/God tools/info/info dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(14.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(posYpercentage, resolution) + gui::p2pX(4.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);
}

void SimulationState::initSaveAsPanel()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(26.0f, resolution);

	this->saveAsPanel = std::make_unique<gui::SaveAsPanel>(
		sf::Vector2f(
			resolution.width,
			resolution.height
		),
		gui::p2pX(34.0f, resolution),
		sf::Color(0, 0, 0, 128)
	);

	this->saveAsPanel->initCenteredText(
		gui::p2pY(39.0f, resolution),
		"ECOSYSTEM NAME:",
		this->fonts["CONSOLAB"],
		charSize,
		sf::Color(225, 225, 255)
	);

	this->saveAsPanel->initInputField(
		sf::Vector2f(
			gui::p2pX(37.0f, resolution),
			gui::p2pY(45.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(26.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["CONSOLAB"], "", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.f / 1080.f, resolution), 0.5f
	);

	this->saveAsPanel->addButton(
		"SAVE",
		sf::Vector2f(
			gui::p2pX(37.0f, resolution),
			gui::p2pY(54.2f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["CONSOLAB"], "SAVE", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->saveAsPanel->addButton(
		"OK",
		sf::Vector2f(
			gui::p2pX(51.0f, resolution),
			gui::p2pY(54.2f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["CONSOLAB"], "OK", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);
}

// other private methods:
void SimulationState::updateInput()
{
	for (const auto& event : *this->stateData->events)
	{
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Key(this->keybinds.at("CLOSE")))
			{
				this->m_sideMenuIsRendered = !this->m_sideMenuIsRendered;
				return;
			}
			if (event.key.code == sf::Keyboard::Key(this->keybinds.at("PAUSE")))
			{
				if (this->stateData->ecosystem->isSimulationPaused())
				{
					this->stateData->ecosystem->unpauseSimulation();
					this->sideMenu->setTextureOfImageButton("PAUSE", "STOP");
				}
				else
				{
					this->stateData->ecosystem->pauseSimulation();
					this->sideMenu->setTextureOfImageButton("PAUSE", "PLAY");
				}
				return;
			}
		}
	}
}

void SimulationState::updateInputWithPanelRendered()
{
	for (const auto& event : *this->stateData->events)
	{
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Key(this->keybinds.at("CLOSE")))
			{
				this->m_sideMenuIsRendered = !this->m_sideMenuIsRendered;
				return;
			}
		}
	}
}

void SimulationState::updateMousePositions(const sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->previousMousePosWindow = this->mousePosWindow;
	this->mousePosWindow = sf::Mouse::getPosition(*this->stateData->window);

	if (view)
	{
		sf::View temp = this->stateData->window->getView();

		this->stateData->window->setView(*view);

		this->mousePosView = this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window));

		this->stateData->window->setView(temp);
	}
	else
		this->mousePosView = this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window));
}

void SimulationState::updateSideMenu()
{
	this->sideMenu->update(this->mousePosWindow, *this->stateData->events);

	this->updateSideMenuGui();
}

void SimulationState::updateSideMenuGui()
{
	// change themes of texture buttons:
	for (auto& it : this->sideMenu->getImageButtons())
	{
		if (it.first == "PAUSE") // pause button:
		{
			if (it.second->getCurrentTextureKey().substr(0, 4) == "PLAY")
			{
				if (it.second->isPressed())
					this->sideMenu->setTextureOfImageButton(it.first, "PLAY PRESSED");

				else if (it.second->isHovered())
					this->sideMenu->setTextureOfImageButton(it.first, "PLAY HOVERED");

				else
					this->sideMenu->setTextureOfImageButton(it.first, "PLAY IDLE");
			}
			else
			{
				if (it.second->isPressed())
					this->sideMenu->setTextureOfImageButton(it.first, "STOP PRESSED");

				else if (it.second->isHovered())
					this->sideMenu->setTextureOfImageButton(it.first, "STOP HOVERED");

				else
					this->sideMenu->setTextureOfImageButton(it.first, "STOP IDLE");
			}
		}
		else if (it.first == "ARROW") // arrow button:
		{
			if (it.second->getCurrentTextureKey().substr(0, 5) == "RIGHT")
			{
				if (it.second->isPressed())
					this->sideMenu->setTextureOfImageButton(it.first, "RIGHT PRESSED");

				else if (it.second->isHovered())
					this->sideMenu->setTextureOfImageButton(it.first, "RIGHT HOVERED");

				else
					this->sideMenu->setTextureOfImageButton(it.first, "RIGHT IDLE");
			}
			else
			{
				if (it.second->isPressed())
					this->sideMenu->setTextureOfImageButton(it.first, "LEFT PRESSED");

				else if (it.second->isHovered())
					this->sideMenu->setTextureOfImageButton(it.first, "LEFT HOVERED");

				else
					this->sideMenu->setTextureOfImageButton(it.first, "LEFT IDLE");
			}
		}
		else if (it.first.substr(0, 4) == "ZOOM")
		{
			if (it.second->isPressed())
				this->sideMenu->setTextureOfImageButton(it.first, "PRESSED");

			else if (it.second->isHovered())
				this->sideMenu->setTextureOfImageButton(it.first, "HOVERED");

			else
				this->sideMenu->setTextureOfImageButton(it.first, "IDLE");
		}
		else // God tools buttons: 
			this->updateGodToolButton(it.first);
	}
}

void SimulationState::updateGodToolButton(const std::string& god_tool_btn_key)
{
	std::string currentGodToolStr = getGodToolStr(this->stateData->ecosystem->getCurrentGodTool());

	if (god_tool_btn_key == currentGodToolStr)
	{
		// if the current tool has been clicked, it is no longer the current tool:
		if (this->sideMenu->getImageButtons().at(god_tool_btn_key)->hasBeenClicked())
		{
			// the button is hovered, because u can't click a button without hovering it with a mouse cursor:
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "LIGHT");

			this->stateData->ecosystem->setGodTool(GodTool::NONE);
		}

		// pretty straight forward, it's pressed so it's dark:
		else if (this->sideMenu->getImageButtons().at(god_tool_btn_key)->isPressed())
		{
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "DARK");
		}

		// is hovered so is light:
		else if (this->sideMenu->getImageButtons().at(god_tool_btn_key)->isHovered())
		{
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "LIGHT");
		}

		// it is neither hovered nor pressed, but let me remind u, that it's still the current tool, so we darken it:
		else
		{
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "DARK");
		}
	}
	else // the argument isn't the current tool:
	{
		// the tool become the current one:
		if (this->sideMenu->getImageButtons().at(god_tool_btn_key)->hasBeenClicked())
		{
			// old tool (if it exists at all) ceases to be the current tool:
			if (this->stateData->ecosystem->getCurrentGodTool() != GodTool::NONE) 
				this->sideMenu->setTextureOfImageButton(currentGodToolStr, "IDLE");

			this->stateData->ecosystem->setGodTool(getGodTool(god_tool_btn_key.c_str()));

			// we brighten it up,
			// because a mouse cursor is still covering it (because it has just been clicked and a mouse hasn't go away yet):
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "LIGHT");
		}

		// next pretty straight forward line, it's hovered so it's light: 
		else if (this->sideMenu->getImageButtons().at(god_tool_btn_key)->isHovered())
		{
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "LIGHT");
		}

		// the most common case, ordinary idle tool:
		else
		{
			this->sideMenu->setTextureOfImageButton(god_tool_btn_key, "IDLE");
		}
	}
}

void SimulationState::getUpdatesFromSideMenuGui()
{
	// get update from side menu texture buttons:
	if (this->sideMenu->getImageButtons().at("PAUSE")->hasBeenClicked())
	{
		if (this->stateData->ecosystem->isSimulationPaused())
			this->stateData->ecosystem->unpauseSimulation();
		else
			this->stateData->ecosystem->pauseSimulation();

		std::string currentTextureKey = this->sideMenu->getImageButtons().at("PAUSE")->getCurrentTextureKey();

		if (currentTextureKey.substr(0, 4) == "PLAY")
			this->sideMenu->setTextureOfImageButton("PAUSE", currentTextureKey.replace(0, 4, "STOP"));

		else
			this->sideMenu->setTextureOfImageButton("PAUSE", currentTextureKey.replace(0, 4, "PLAY"));
	}

	if (this->sideMenu->getImageButtons().at("ARROW")->hasBeenClicked())
	{
		std::string currentTextureKey = this->sideMenu->getImageButtons().at("ARROW")->getCurrentTextureKey();

		if (currentTextureKey.substr(0, 5) == "RIGHT")
		{
			this->sideMenu->setPosition(
				sf::Vector2f(
					this->stateData->gfxSettings->resolution.width - this->sideMenu->getSize().x,
					0.f
				)
			);

			this->sideMenu->setTextureOfImageButton("ARROW", currentTextureKey.replace(0, 5, "LEFT"));
		}
		else
		{
			this->sideMenu->setPosition(
				sf::Vector2f(
					0.f,
					0.f
				)
			);

			this->sideMenu->setTextureOfImageButton("ARROW", currentTextureKey.replace(0, 4, "RIGHT"));
		}
	}

	if (this->sideMenu->getImageButtons().at("ZOOM IN")->isPressed())
		this->view.zoom(0.95f);

	if (this->sideMenu->getImageButtons().at("ZOOM OUT")->isPressed())
		this->view.zoom(1.0f / 0.95f);

	// get update from side menu buttons:
	if (this->sideMenu->getButtons().at("SAVE")->isClicked())
		this->stateData->ecosystem->saveToFolder("ecosystems/" + this->stateData->ecosystem->getName());

	else if (this->sideMenu->getButtons().at("SAVE AS")->isClicked())
		this->m_saveAsPanelIsRendered = true;

	else if (this->sideMenu->getButtons().at("QUIT")->isClicked()) 
		this->endState();
}

void SimulationState::getUpdatesFromSaveAsPanel()
{
	if (this->saveAsPanel->getButton("SAVE")->isClicked())
		this->stateData->ecosystem->saveToFolder(
			"ecosystems/" + this->saveAsPanel->getInputField()->getInput()
		);

	else if (this->saveAsPanel->getButton("OK")->isClicked())
		this->m_saveAsPanelIsRendered = false;
}

void SimulationState::updateView()
{
	// TODO: move that to updateInput method? And put into a separate function?:
	// zoom view:
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) this->view.zoom(0.95f);

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) this->view.zoom(1.0f / 0.95f);

	// move view:
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	if (
		sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		!this->sideMenu->getBackground().getGlobalBounds().contains(static_cast<sf::Vector2f>(this->mousePosWindow)))
	{
		int offsetX = this->previousMousePosWindow.x - this->mousePosWindow.x;
		int offsetY = this->previousMousePosWindow.y - this->mousePosWindow.y;

		this->view.move(
			offsetX * this->view.getSize().x / this->stateData->gfxSettings->resolution.width,
			offsetY * this->view.getSize().y / this->stateData->gfxSettings->resolution.height
		);
	}

	// change the center of view if an animal is tracked:
	if (this->stateData->ecosystem->getTrackedAnimal())
		this->view.setCenter(this->stateData->ecosystem->getTrackedAnimal()->getPosition());

	// correct zoom:
	float worldWidth = static_cast<float>(this->stateData->ecosystem->getWorldSize().x);
	float worldHeight = static_cast<float>(this->stateData->ecosystem->getWorldSize().y);

	this->view.setSize(
		std::min(this->view.getSize().x, worldWidth),
		std::min(this->view.getSize().y, worldHeight)
	);

	// correct view moving:
	if (this->view.getCenter().x - this->view.getSize().x / 2.f < 0.f)
		this->view.setCenter(this->view.getSize().x / 2.f, this->view.getCenter().y);

	if (this->view.getCenter().x + this->view.getSize().x / 2.f > worldWidth)
		this->view.setCenter(worldWidth - this->view.getSize().x / 2.f, this->view.getCenter().y);

	if (this->view.getCenter().y - this->view.getSize().y / 2.f < 0.f)
		this->view.setCenter(this->view.getCenter().x, this->view.getSize().y / 2.f);

	if (this->view.getCenter().y + this->view.getSize().y / 2.f > worldHeight)
		this->view.setCenter(this->view.getCenter().x, worldHeight - this->view.getSize().y / 2.f);
}

void SimulationState::updateEcosystem(float dt)
{
	this->stateData->ecosystem->setSimulationSpeedFactor(
		this->sideMenu->getSliders().at("SPEED")->getCurrentValue()
	);

	this->useEcosystemGodTools();

	this->stateData->ecosystem->update(dt);
}

void SimulationState::useEcosystemGodTools()
{
	sf::FloatRect sideMenuGlobalBounds = this->sideMenu->getBackground().getGlobalBounds();

	sf::Vector2f mousePosWindow = static_cast<sf::Vector2f>(this->mousePosWindow);
	
	if (this->m_sideMenuIsRendered && sideMenuGlobalBounds.contains(mousePosWindow))
		return;

	this->stateData->ecosystem->useGodTools(*this->stateData->events, this->mousePosView);
}
