#include "SimulationState.h"

SimulationState::SimulationState(StateData* stateData)
	: State(stateData)
	, m_fonts()
	, m_view()
	, m_previousMousePosWindow()
	, m_sideMenuIsRendered(false)
	, m_sideMenu(nullptr)
	, m_renderTexture()
	, m_renderSprite()
	, m_brainPreviewModifier(nullptr)
	, m_saveAsPanelIsRendered(false)
	, m_saveAsPanel(nullptr)
{
	initKeybinds();
	initVariables();
	initFonts();
	initEcosystem();
	initView();
	initDeferredRender();
	initSideMenu();
	initBrainPreviewModifier();
	initSaveAsPanel();
}

void SimulationState::update(float dt)
{
	if (!m_saveAsPanelIsRendered)
	{
		updateInput();
	}
	else
	{
		updateInputWithPanelRendered();
	}

	updateMousePositions(&m_view);

	updateSideMenuVisibility();

	if (m_sideMenuIsRendered)
	{
		updateSideMenu();
	}

	getUpdatesFromSideMenuGui();

	if (m_saveAsPanelIsRendered)
	{
		m_saveAsPanel->update(
			dt, 
			*m_stateData->m_events, 
			m_mousePosWindow
		);
	}

	getUpdatesFromSaveAsPanel();

	updateView();

	updateEcosystem(dt);
	
	updateBrainPreviewModifier();
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = m_stateData->m_window;
	}

	m_renderTexture.clear();

	// draw ecosystem:
	m_renderTexture.setView(m_view);

	m_stateData->m_ecosystem->render(m_renderTexture);

	// render side menu, brain preview modifier and save as panel:
	m_renderTexture.setView(m_renderTexture.getDefaultView());

	if (m_sideMenuIsRendered)
	{
		m_sideMenu->render(m_renderTexture);
	}

	if (m_stateData->m_ecosystem->getAnimalWithModifiedBrain())
	{
		m_brainPreviewModifier->render(
			m_stateData->m_ecosystem->getAnimalWithModifiedBrain()->getBrainPreview(),
			m_renderTexture
		);
	}

	if (m_saveAsPanelIsRendered)
	{
		m_saveAsPanel->render(m_renderTexture);
	}

	// final render:
	m_renderTexture.display();

	target->draw(m_renderSprite);
}

// mutators:

void SimulationState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

// private methods:

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
		{
			m_keybinds[key] = m_stateData->m_supportedKeys->at(key2);
		}
	}
	else
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::SimulationState::initKeybinds()::"
			<< "could not open "
			<< path + '\n'
		);
	}

	ifs.close();
}

void SimulationState::initVariables()
{
	m_sideMenu = nullptr;
	m_sideMenuIsRendered = false;
	m_saveAsPanelIsRendered = false;
	m_previousMousePosWindow = sf::Vector2i(0, 0);
}

void SimulationState::initFonts()
{
	if (!m_fonts["Retroica"].loadFromFile("resources/fonts/Retroica.ttf")
		|| !m_fonts["CONSOLAB"].loadFromFile("resources/fonts/CONSOLAB.ttf"))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::SimulationState::initFonts()::"
			<< "could not load a font\n"
		);
	}
}

void SimulationState::initEcosystem()
{
	m_stateData->m_ecosystem->pauseSimulation();
	m_stateData->m_ecosystem->printInfo();
	std::cout << '\n';
}

void SimulationState::initView()
{
	m_view.setSize(m_stateData->m_ecosystem->getWorldSize());

	m_view.setCenter(
		sf::Vector2f(
			m_stateData->m_ecosystem->getWorldSize().x / 2.f,
			m_stateData->m_ecosystem->getWorldSize().y / 2.f
		)
	);
}

void SimulationState::initDeferredRender()
{
	m_renderTexture.create(
		m_stateData->m_gfxSettings->resolution.width,
		m_stateData->m_gfxSettings->resolution.height
	);

	m_renderSprite.setTexture(m_renderTexture.getTexture());
	m_renderSprite.setTextureRect(
		sf::IntRect(
			0,
			0,
			m_stateData->m_gfxSettings->resolution.width,
			m_stateData->m_gfxSettings->resolution.height
		)
	);
}

void SimulationState::initSideMenu()
{
	// temporary variables:
	const std::string guiPath = "resources/textures/GUI";

	const sf::VideoMode resolution = m_stateData->m_gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(24.0f, resolution);

	// create new SideMenu:
	m_sideMenu = std::make_unique<gui::SideMenu>(
		sf::Vector2f(
			0.0f, 
			0.0f
		),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution), 
			gui::p2pY(100.0f, resolution)
		),
		sf::Color(48, 48, 48)
	);

	// add widgets:
	m_sideMenu->addCenteredText(
		gui::p2pY(4.0f, resolution),
		charSize,
		m_fonts["CONSOLAB"],
		"PLAY/STOP:",
		sf::Color(225, 225, 225)
	);
	m_sideMenu->addImageButton(
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
	
	m_sideMenu->addCenteredText(
		gui::p2pY(17.5f, resolution),
		charSize,
		m_fonts["CONSOLAB"],
		"SPEED:",
		sf::Color(225, 225, 225)
	);
	m_sideMenu->addSlider(
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

	m_sideMenu->addCenteredText(
		gui::p2pY(28.5f, resolution),
		charSize,
		m_fonts["CONSOLAB"],
		"MOVE THIS PANEL:",
		sf::Color(225, 225, 225)
	);
	m_sideMenu->addImageButton(
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

	initGodToolsGui();

	m_sideMenu->addCenteredText(
		gui::p2pY(61.0f, resolution),
		charSize,
		m_fonts["CONSOLAB"],
		"ZOOM:",
		sf::Color(225, 225, 225)
	);
	m_sideMenu->addImageButton(
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

	m_sideMenu->addImageButton(
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

	m_sideMenu->addButton(
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
		m_fonts["CONSOLAB"],
		"SAVE",
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_sideMenu->addButton(
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
		m_fonts["CONSOLAB"],
		"SAVE AS",
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_sideMenu->addButton(
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
		m_fonts["CONSOLAB"],
		"QUIT",
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64),
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);
}

void SimulationState::initGodToolsGui()
{
	// temporary variables:
	const std::string& guiPath = "resources/textures/GUI";

	const sf::VideoMode resolution = m_stateData->m_gfxSettings->resolution;

	float posYpercentage = 45.5f;

	// init God tools GUI:
	m_sideMenu->addCenteredText(
		gui::p2pY(41.5f, resolution),
		gui::calcCharSize(24.0f, resolution),
		m_fonts["CONSOLAB"],
		"GOD TOOLS:",
		sf::Color(225, 225, 225)
	);

	m_sideMenu->addImageButton(
		"MUTATE",
		{
			{"IDLE", guiPath + "/God tools/mutate/mutate.png"},
			{"LIGHT", guiPath + "/God tools/mutate/mutate light.png"},
			{"DARK", guiPath + "/God tools/mutate/mutate dark.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(2.0f + 1.0f / 3.0f, resolution),
			gui::p2pY(posYpercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.0f * 64.0f / 1920.0f, resolution),
			gui::p2pY(100.0f * 64.0f / 1080.0f, resolution)
		)
	);

	m_sideMenu->addImageButton(
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

	m_sideMenu->addImageButton(
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

	m_sideMenu->addImageButton(
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

	m_sideMenu->addImageButton(
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

	m_sideMenu->addImageButton(
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

	m_sideMenu->addImageButton(
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
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(26.0f, resolution);

	m_saveAsPanel = std::make_unique<gui::SaveAsPanel>(
		sf::Vector2f(
			resolution.width,
			resolution.height
		),
		gui::p2pX(34.0f, resolution),
		sf::Color(0, 0, 0, 128)
	);

	m_saveAsPanel->initCenteredText(
		gui::p2pY(39.0f, resolution),
		"ECOSYSTEM NAME:",
		m_fonts["CONSOLAB"],
		charSize,
		sf::Color(225, 225, 255)
	);

	m_saveAsPanel->initTextBox(
		sf::Vector2f(
			gui::p2pX(37.0f, resolution),
			gui::p2pY(45.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(26.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_fonts["CONSOLAB"], 
		"", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.f / 1080.f, resolution), 
		0.5f
	);

	m_saveAsPanel->addButton(
		"SAVE",
		sf::Vector2f(
			gui::p2pX(37.0f, resolution),
			gui::p2pY(54.2f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_fonts["CONSOLAB"], 
		"SAVE", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_saveAsPanel->addButton(
		"OK",
		sf::Vector2f(
			gui::p2pX(51.0f, resolution),
			gui::p2pY(54.2f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_fonts["CONSOLAB"], 
		"OK", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);
}

void SimulationState::initBrainPreviewModifier()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;
	
	const sf::Vector2f size{
		0.75f * resolution.height,
		0.75f * resolution.height
	};

	m_brainPreviewModifier = std::make_unique<gui::BrainPreviewModifier>(
		sf::Vector2f(
			(resolution.width - size.x) / 2.0f,
			(resolution.height - size.y) / 2.0f
		),
		size
	);
}

// utils:

void SimulationState::updateInput()
{
	for (const auto& event : *m_stateData->m_events)
	{
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Key(m_keybinds.at("CLOSE")))
			{
				m_sideMenuIsRendered = !m_sideMenuIsRendered;
				return;
			}
			if (event.key.code == sf::Keyboard::Key(m_keybinds.at("PAUSE")))
			{
				if (m_stateData->m_ecosystem->isSimulationPaused())
				{
					m_stateData->m_ecosystem->unpauseSimulation();
					m_sideMenu->setTextureOfImgBtn("PAUSE", "STOP");
				}
				else
				{
					m_stateData->m_ecosystem->pauseSimulation();
					m_sideMenu->setTextureOfImgBtn("PAUSE", "PLAY");
				}
				return;
			}
			if (event.key.code == sf::Keyboard::Key(m_keybinds["BRAINS"]))
			{
				auto brainsVisibility = m_stateData->m_ecosystem->getBrainsVisibility();

				if (brainsVisibility.empty()) return;

				if (brainsVisibility.begin()->second)
				{
					m_stateData->m_ecosystem->hideAllBrainsPreviews();
					return;
				}

				m_stateData->m_ecosystem->showAllBrainsPreviews();
				return;
			}
		}
	}
}

void SimulationState::updateInputWithPanelRendered()
{
	for (const auto& event : *m_stateData->m_events)
	{
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Key(m_keybinds["CLOSE"]))
			{
				m_sideMenuIsRendered = !m_sideMenuIsRendered;
				return;
			}
			if (event.key.code == sf::Keyboard::Key(m_keybinds["BRAINS"]))
			{
				auto brainsVisibility = m_stateData->m_ecosystem->getBrainsVisibility();
				
				if (brainsVisibility.empty()) return;

				if (brainsVisibility.begin()->second)
				{
					m_stateData->m_ecosystem->hideAllBrainsPreviews();
					return;
				}
				
				m_stateData->m_ecosystem->showAllBrainsPreviews();
				return;
			}
		}
	}
}

void SimulationState::updateMousePositions(const sf::View* view)
{
	m_mousePosScreen = sf::Mouse::getPosition();
	m_previousMousePosWindow = m_mousePosWindow;
	m_mousePosWindow = sf::Mouse::getPosition(*m_stateData->m_window);

	if (view)
	{
		sf::View temp = m_stateData->m_window->getView();

		m_stateData->m_window->setView(*view);

		m_mousePosView = m_stateData->m_window->mapPixelToCoords(
			sf::Mouse::getPosition(*m_stateData->m_window)
		);

		m_stateData->m_window->setView(temp);
	}
	else
	{
		m_mousePosView = m_stateData->m_window->mapPixelToCoords(
			sf::Mouse::getPosition(*m_stateData->m_window)
		);
	}
}

void SimulationState::updateSideMenuVisibility()
{
	if (m_sideMenu->getPosition().x == 0.0f)
	{
		if (m_mousePosWindow.x <= 0.0f && m_previousMousePosWindow.x > 0.0f)
		{
			m_sideMenuIsRendered = !m_sideMenuIsRendered;
		}
	}
	else
	{
		unsigned winWidth = m_stateData->m_window->getSize().x;
			
		if (m_mousePosWindow.x >= winWidth - 1
			&& m_previousMousePosWindow.x < winWidth - 1)
		{
			m_sideMenuIsRendered = !m_sideMenuIsRendered;
		}
	}
}

void SimulationState::updateSideMenu()
{
	m_sideMenu->update(
		static_cast<sf::Vector2f>(m_mousePosWindow), 
		*m_stateData->m_events
	);

	updateSideMenuGui();
}

void SimulationState::updateSideMenuGui()
{
	// change themes of texture buttons:
	for (auto& imgBtn : m_sideMenu->getImageButtons())
	{
		if (imgBtn.first == "PAUSE") // pause button:
		{
			if (imgBtn.second->getCurrentTextureKey().substr(0, 4) == "PLAY")
			{
				if (imgBtn.second->isPressed())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"PLAY PRESSED"
					);
				}
				else if (imgBtn.second->isHovered())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"PLAY HOVERED"
					);
				}
				else
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"PLAY IDLE"
					);
				}
			}
			else
			{
				if (imgBtn.second->isPressed())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"STOP PRESSED"
					);
				}
				else if (imgBtn.second->isHovered())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"STOP HOVERED"
					);
				}
				else
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"STOP IDLE"
					);
				}
			}
		}
		else if (imgBtn.first == "ARROW") // arrow button:
		{
			if (imgBtn.second->getCurrentTextureKey().substr(0, 5) == "RIGHT")
			{
				if (imgBtn.second->isPressed())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"RIGHT PRESSED"
					);
				}
				else if (imgBtn.second->isHovered())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"RIGHT HOVERED"
					);
				}
				else
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"RIGHT IDLE"
					);
				}
			}
			else
			{
				if (imgBtn.second->isPressed())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"LEFT PRESSED"
					);
				}
				else if (imgBtn.second->isHovered())
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"LEFT HOVERED"
					);
				}
				else
				{
					m_sideMenu->setTextureOfImgBtn(
						imgBtn.first, 
						"LEFT IDLE"
					);
				}
			}
		}
		else if (imgBtn.first.substr(0, 4) == "ZOOM")
		{
			if (imgBtn.second->isPressed())
			{
				m_sideMenu->setTextureOfImgBtn(
					imgBtn.first, 
					"PRESSED"
				);
			}
			else if (imgBtn.second->isHovered())
			{
				m_sideMenu->setTextureOfImgBtn(
					imgBtn.first, 
					"HOVERED"
				);
			}
			else
			{
				m_sideMenu->setTextureOfImgBtn(
					imgBtn.first, 
					"IDLE"
				);
			}
		}
		else // God tools buttons: 
		{
			updateGodToolButton(imgBtn.first);
		}
	}
}

void SimulationState::updateGodToolButton(const std::string& godToolBtnKey)
{
	Ecosystem* ecosystem = m_stateData->m_ecosystem;

	std::string currentGodToolStr = getGodToolStr(
		ecosystem->getCurrentGodTool()
	);

	if (godToolBtnKey == currentGodToolStr)
	{
		// if the current tool has been clicked, 
		// it is no longer the current tool:
		if (m_sideMenu->getImageButtons().at(godToolBtnKey)->hasBeenClicked())
		{
			// the button is hovered, 
			// because u can't click a button 
			// without hovering it with a mouse cursor:
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "LIGHT");

			ecosystem->setGodTool(GodTool::NONE);
		}

		// pretty straight forward, it's pressed so it's dark:
		else if (m_sideMenu->getImageButtons().at(godToolBtnKey)->isPressed())
		{
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "DARK");
		}

		// is hovered so is light:
		else if (m_sideMenu->getImageButtons().at(godToolBtnKey)->isHovered())
		{
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "LIGHT");
		}

		// it is neither hovered nor pressed, but let me remind u,
		// that it's still the current tool, so we darken it:
		else
		{
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "DARK");
		}
	}
	else // the argument isn't the current tool:
	{
		// the tool become the current one:
		if (m_sideMenu->getImageButtons().at(godToolBtnKey)->hasBeenClicked())
		{
			// old tool (if it exists at all) ceases to be the current tool:
			if (ecosystem->getCurrentGodTool() != GodTool::NONE)
			{
				m_sideMenu->setTextureOfImgBtn(
					currentGodToolStr, 
					"IDLE"
				);
			}

			ecosystem->setGodTool(getGodTool(godToolBtnKey.c_str()));

			// we brighten it up,
			// because a mouse cursor is still covering it 
			// (because it has just been clicked 
			// and a mouse hasn't go away yet):
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "LIGHT");
		}

		// next pretty straight forward line, it's hovered so it's light: 
		else if (m_sideMenu->getImageButtons().at(godToolBtnKey)->isHovered())
		{
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "LIGHT");
		}

		// the most common case, ordinary idle tool:
		else
		{
			m_sideMenu->setTextureOfImgBtn(godToolBtnKey, "IDLE");
		}
	}
}

void SimulationState::getUpdatesFromSideMenuGui()
{
	typedef std::unique_ptr<gui::ImageButton> ImgBtnUniquePtr;
	typedef std::unordered_map<std::string, ImgBtnUniquePtr> ImgBtnsMap;

	const ImgBtnsMap& sideMenuImgBtns = m_sideMenu->getImageButtons();

	// get update from side menu texture buttons:
	if (sideMenuImgBtns.at("PAUSE")->hasBeenClicked())
	{
		if (m_stateData->m_ecosystem->isSimulationPaused())
		{
			m_stateData->m_ecosystem->unpauseSimulation();
		}
		else
		{
			m_stateData->m_ecosystem->pauseSimulation();
		}

		const ImgBtnUniquePtr& imgBtn = sideMenuImgBtns.at("PAUSE");
		std::string currentTextureKey = imgBtn->getCurrentTextureKey();

		if (currentTextureKey.substr(0, 4) == "PLAY")
		{
			m_sideMenu->setTextureOfImgBtn(
				"PAUSE",
				currentTextureKey.replace(0, 4, "STOP")
			);
		}
		else
		{
			m_sideMenu->setTextureOfImgBtn(
				"PAUSE", 
				currentTextureKey.replace(0, 4, "PLAY")
			);
		}
	}

	if (sideMenuImgBtns.at("ARROW")->hasBeenClicked())
	{
		const ImgBtnUniquePtr& imgBtn = sideMenuImgBtns.at("ARROW");
		std::string currentTextureKey = imgBtn->getCurrentTextureKey();

		if (currentTextureKey.substr(0, 5) == "RIGHT")
		{
			const GraphicsSettings* gfxSettings = m_stateData->m_gfxSettings;

			m_sideMenu->setPosition(
				sf::Vector2f(
					gfxSettings->resolution.width - m_sideMenu->getSize().x,
					0.f
				)
			);

			m_sideMenu->setTextureOfImgBtn(
				"ARROW", 
				currentTextureKey.replace(0, 5, "LEFT")
			);
		}
		else
		{
			m_sideMenu->setPosition(
				sf::Vector2f(
					0.f,
					0.f
				)
			);

			m_sideMenu->setTextureOfImgBtn(
				"ARROW", 
				currentTextureKey.replace(0, 4, "RIGHT")
			);
		}
	}

	if (sideMenuImgBtns.at("ZOOM IN")->isPressed())
	{
		m_view.zoom(0.95f); // TODO: "unhardcode" this
	}

	if (sideMenuImgBtns.at("ZOOM OUT")->isPressed())
	{
		m_view.zoom(1.0f / 0.95f); // TODO: "unhardcode" this
	}

	// get update from side menu buttons:
	if (m_sideMenu->getButtons().at("SAVE")->isClicked())
	{
		m_stateData->m_ecosystem->saveToFolder(
			"ecosystems/" + m_stateData->m_ecosystem->getName()
		);
	}
	else if (m_sideMenu->getButtons().at("SAVE AS")->isClicked())
	{
		m_saveAsPanelIsRendered = true;
	}
	else if (m_sideMenu->getButtons().at("QUIT")->isClicked())
	{
		endState();
	}
}

void SimulationState::getUpdatesFromSaveAsPanel()
{
	if (m_saveAsPanel->getButton("SAVE")->isClicked())
	{
		m_stateData->m_ecosystem->saveToFolder(
			"ecosystems/" + m_saveAsPanel->getTextBox()->getInput()
		);
	}
	else if (m_saveAsPanel->getButton("OK")->isClicked())
	{
		m_saveAsPanelIsRendered = false;
	}
}

void SimulationState::updateView()
{
	// TODO: move that to updateInput method? 
	// TODO: And put into a separate function?:
	
	// zoom view:
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["ZOOM_IN"])))
	{
		m_view.zoom(0.95f); // TODO: "unhardcode" this
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["ZOOM_OUT"])))
	{
		m_view.zoom(1.0f / 0.95f); // TODO: "unhardcode" this
	}

	// move view:
	sf::RectangleShape sideMenuBg = m_sideMenu->getBackground();
	auto mousePosWindowF = static_cast<sf::Vector2f>(m_mousePosWindow);
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) 
		&& !sideMenuBg.getGlobalBounds().contains(mousePosWindowF))
	{
		int offsetX = m_previousMousePosWindow.x - m_mousePosWindow.x;
		int offsetY = m_previousMousePosWindow.y - m_mousePosWindow.y;

		sf::VideoMode resolution = m_stateData->m_gfxSettings->resolution;
			
		m_view.move(
			offsetX * m_view.getSize().x / resolution.width,
			offsetY * m_view.getSize().y / resolution.height
		);
	}

	// change the center of view if an animal is tracked:
	if (m_stateData->m_ecosystem->getTrackedAnimal())
	{
		m_view.setCenter(
			m_stateData->m_ecosystem->getTrackedAnimal()->getPos()
		);
	}

	float worldWidth = static_cast<float>(
		m_stateData->m_ecosystem->getWorldSize().x
		);
	float worldHeight = static_cast<float>(
		m_stateData->m_ecosystem->getWorldSize().y
		);

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["DEVELOPER_MODE_ZOOMING_OUT"])))
	{
		// correct zoom:
		m_view.setSize(
			std::min(
				m_view.getSize().x,
				worldWidth
			),
			std::min(
				m_view.getSize().y,
				worldHeight
			)
		);

		// correct view moving:
		if (m_view.getCenter().x - m_view.getSize().x / 2.f < 0.f)
		{
			m_view.setCenter(
				m_view.getSize().x / 2.f,
				m_view.getCenter().y
			);
		}
		if (m_view.getCenter().x + m_view.getSize().x / 2.f > worldWidth)
		{
			m_view.setCenter(
				worldWidth - m_view.getSize().x / 2.f,
				m_view.getCenter().y
			);
		}
		if (m_view.getCenter().y - m_view.getSize().y / 2.f < 0.f)
		{
			m_view.setCenter(
				m_view.getCenter().x,
				m_view.getSize().y / 2.f
			);
		}
		if (m_view.getCenter().y + m_view.getSize().y / 2.f > worldHeight)
		{
			m_view.setCenter(
				m_view.getCenter().x,
				worldHeight - m_view.getSize().y / 2.f
			);
		}
	}
}

void SimulationState::updateEcosystem(float dt)
{
	m_stateData->m_ecosystem->setSimulationSpeedFactor(
		m_sideMenu->getSliders().at("SPEED")->getCurrentValue()
	);

	useEcosystemGodTools();

	m_stateData->m_ecosystem->update(
		dt, 
		m_mousePosView, 
		*m_stateData->m_events,
		m_keybinds,
		allowUpdatingModifyingBrainPreviews()
	);
}

void SimulationState::useEcosystemGodTools()
{
	sf::RectangleShape sideMenuBg = m_sideMenu->getBackground();
	auto mousePosWinF = static_cast<sf::Vector2f>(m_mousePosWindow);

	if (m_sideMenuIsRendered
		&& sideMenuBg.getGlobalBounds().contains(mousePosWinF)) return;

	if (m_stateData->m_ecosystem->getAnimalWithModifiedBrain()
		&& m_brainPreviewModifier->getBgBounds().contains(mousePosWinF)) return;
		
	m_stateData->m_ecosystem->useGodTools(
		*m_stateData->m_events, 
		m_mousePosView
	);
}

bool SimulationState::allowUpdatingModifyingBrainPreviews() const
{
	sf::Vector2f mousePosWindow = static_cast<sf::Vector2f>(m_mousePosWindow);

	return !m_brainPreviewModifier->getBgBounds().contains(mousePosWindow)
		|| !m_stateData->m_ecosystem->getAnimalWithModifiedBrain();
}

void SimulationState::updateBrainPreviewModifier()
{	
	if (!m_stateData->m_ecosystem->getAnimalWithModifiedBrain()) return;
	
	m_brainPreviewModifier->update(
		m_stateData->m_ecosystem->getAnimalWithModifiedBrain()->getBrainPreview(),
		static_cast<sf::Vector2f>(m_mousePosWindow),
		*m_stateData->m_events
	);

	if (m_brainPreviewModifier->getCloseBtn()->hasBeenClicked())
	{
		m_stateData->m_ecosystem->stopModifyingBrainPreview();
		return;
	}	
}
