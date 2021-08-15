#include "pch.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateData* stateData)
	: State(stateData)
	, m_backgroundRect()
	, m_fonts()
	, m_buttons()
	, m_ecosystemText()
	, m_defaultEcosystemTextColor()
	, m_highlightedEcosystemTextColor()
	, m_ecosystemTextStopwatch(0.0f)
	, m_highlightningTime(0.0f)
{
	initVariables();
	initKeybinds();
	initBackground();
	initFonts();
	initButtons();
	initEcosystemText();
}

void MainMenuState::update(float dt)
{
	updateMousePositions();
	updateInput();
	getUpdateFromButtons();
	updateEcosystemText(dt);
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = m_stateData->m_window;
	}

	target->draw(m_backgroundRect);

	renderButtons(*target);

	target->draw(m_ecosystemText);
}

// mutators:

void MainMenuState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

// private methods:

void MainMenuState::initVariables()
{
	m_defaultEcosystemTextColor = sf::Color(216, 216, 216);
	m_highlightedEcosystemTextColor = sf::Color::Red;

	m_ecosystemTextStopwatch = 0.0f;
	m_highlightningTime = 0.5f;
}

void MainMenuState::initKeybinds()
{
	const char* path = "config/main_menu_keybinds.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			m_keybinds[key] = m_stateData->m_supportedKeys->at(key2);
		}
	}
	else
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::MainMenuState::initKeybinds()"
			<< "could not open "
			<< path << '\n'
		);
	}

	ifs.close();
}

void MainMenuState::initBackground()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_backgroundRect.setSize(
		sf::Vector2f
		(
			static_cast<float>(resolution.width),
			static_cast<float>(resolution.height)
		)
	);

	m_backgroundRect.setFillColor(sf::Color(32, 32, 32));
}

void MainMenuState::initFonts()
{
	if (!m_fonts["Retroica"].loadFromFile("resources/fonts/Retroica.ttf")
		|| !m_fonts["CONSOLAB"].loadFromFile("resources/fonts/CONSOLAB.ttf"))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::MainMenuState::initFonts()::"
			<< "could not load a font\n"
		);
	}
}

void MainMenuState::initButtons()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_buttons["SIMULATE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution),
			gui::p2pY(31.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(8.f, resolution)
		),
		m_fonts["Retroica"], 
		"SIMULATE", 
		gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	m_buttons["NEW ECOSYSTEM"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(43.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(8.f, resolution)
		),
		m_fonts["Retroica"], 
		"NEW ECOSYSTEM", 
		gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100),
		sf::Color(125, 125, 125),
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	m_buttons["LOAD"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(55.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(8.f, resolution)
		),
		m_fonts["Retroica"], 
		"LOAD", 
		gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	m_buttons["QUIT"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(67.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(8.f, resolution)
		),
		m_fonts["Retroica"], 
		"QUIT",
		gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64),
		sf::Color(100, 100, 100),
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225),
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);
}

void MainMenuState::initEcosystemText()
{
	m_ecosystemText.setFont(m_fonts["CONSOLAB"]);
	m_ecosystemText.setString("NO STRING HAS BEEN SET FOR THIS TEXT");
	m_ecosystemText.setPosition(50.0f, 50.0f);
	m_ecosystemText.setCharacterSize(32U);
	m_ecosystemText.setFillColor(m_defaultEcosystemTextColor);
}

void MainMenuState::highlightEcosystemText()
{
	m_ecosystemText.setFillColor(m_highlightedEcosystemTextColor);
	m_ecosystemTextStopwatch = 0.0f;
}

void MainMenuState::saveEcosystem(const Ecosystem& ecosystem)
{
	std::cout << "SAVE IS NOT DEFINET YET\n";
}

void MainMenuState::updateInput()
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["CLOSE"]))
	//	&& !keysBlockades["CLOSE"])
	//{
	//	endState();
	//}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["CLOSE"])))
	{
		endState();
	}
}

void MainMenuState::getUpdateFromButtons()
{
	for (auto& btn : m_buttons)
	{
		btn.second->update(m_mousePosWindow);
	}

	if (m_buttons["SIMULATE"]->isClicked())
	{
		// TODO: What about this?:
		//if (m_stateData->m_ecosystem->isInitialized())
		//{
		//	m_stateData->m_states->push(new SimulationState(m_stateData));
		//	m_stateData->m_states->top()->freeze();
		//}
		//else
		//{
		//	highlightEcosystemText();
		//}
		if (m_stateData->m_ecosystem)
		{
			m_stateData->m_states->push(new SimulationState(m_stateData));
			m_stateData->m_states->top()->freeze();
		}
	}
	else if (m_buttons["NEW ECOSYSTEM"]->isClicked())
	{
		m_stateData->m_states->push(new EcosystemCreatorState(m_stateData));
		m_stateData->m_states->top()->freeze();
	}
	else if (m_buttons["LOAD"]->isClicked())
	{
		m_stateData->m_states->push(new LoadingState(m_stateData));
		m_stateData->m_states->top()->freeze();
	}
	else if (m_buttons["QUIT"]->isClicked())
	{
		endState();
	}
}

void MainMenuState::updateEcosystemText(float dt)
{
	// TODO: ecosystem text is not updated at all! Change that!:
	//if (!m_stateData->m_ecosystem->isInitialized())
	//{
	//	m_ecosystemText.setString(
	//		"CREATE A NEW ECOSYSTEM OR LOAD AN EXISTING ONE"
	//	);
	//
	//	if (m_ecosystemText.getFillColor() == m_highlightedEcosystemTextColor)
	//	{
	//		if (m_ecosystemTextStopwatch > m_highlightningTime)
	//		{
	//			m_ecosystemText.setFillColor(m_defaultEcosystemTextColor);
	//		}
	//
	//		m_ecosystemTextStopwatch += dt;
	//	}
	//}
	//else
	//{
	//	m_ecosystemText.setString(
	//		"CURRENT ECOSYSTEM FOLDER: " 
	//		+ m_stateData->m_ecosystem->getName()
	//	);
	//}	
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& button : m_buttons)
	{
		button.second->render(target);
	}
}
