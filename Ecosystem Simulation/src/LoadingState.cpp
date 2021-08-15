#include "pch.h"
#include "LoadingState.h"

LoadingState::LoadingState(StateData* stateData)
	: State(stateData)
	, m_background()
	, m_container()
	, m_font()
	, m_texts()
	, m_textBox()
	, m_buttons()
{
	initKeybinds();
	initBackground();
	initContainer();
	initFonts();
	initTexts();
	initTextBox();
	initButtons();
}

void LoadingState::update(float dt)
{
	updateMousePositions();

	updateInput();

	m_textBox->update(dt, *m_stateData->m_events, m_mousePosWindow);

	for (auto& button : m_buttons)
	{
		button.second->update(m_mousePosWindow);
	}

	getUpdateFromButtons();
}

void LoadingState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = m_stateData->m_window;
	}

	target->draw(m_background);
	target->draw(m_container);

	for (const auto& text : m_texts)
	{
		target->draw(*text.second);
	}

	m_textBox->render(*target);

	for (const auto& button : m_buttons)
	{
		button.second->render(*target);
	}
}

// mutators:

void LoadingState::freeze()
{
	std::cout << "FREEZING IS NOT DEFINED YET!\n";

	for (auto& button : m_buttons)
	{
		button.second->setClickBlockade(true);
	}
}

// private methods:

void LoadingState::initKeybinds()
{
	const char* path = "config/ecosystem_creator_keybinds.ini";

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
			<< "Error::LoadingState::initKeybinds()::"
			<< "could not open "
			<< path << '\n'
		);
	}

	ifs.close();
}

void LoadingState::initBackground()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_background.setSize(
		sf::Vector2f
		(
			static_cast<float>(resolution.width),
			static_cast<float>(resolution.height)
		)
	);

	m_background.setFillColor(sf::Color(28, 28, 28));
}

void LoadingState::initContainer()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_container = sf::RectangleShape(
		sf::Vector2f(
			gui::p2pX(34.0f, resolution),
			gui::p2pY(100.0f, resolution)
		)
	);

	m_container.setFillColor(sf::Color(0, 0, 0, 128));
	
	sf::FloatRect containerBounds = m_container.getGlobalBounds();

	m_container.setPosition(
		sf::Vector2f(
			resolution.width / 2.0f - containerBounds.width / 2.0f,
			0.0f
		)
	);
}

void LoadingState::initFonts()
{
	if (!m_font.loadFromFile("resources/fonts/consolab.ttf"))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::LoadingState::initFonts()::"
			<< "cannot load a font\n"
		);
	}
}

void LoadingState::initTexts()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_texts["ECOSYSTEM NAME"] = std::make_unique<sf::Text>(
		"ECOSYSTEM NAME:",
		m_font,
		gui::calcCharSize(32.0f, resolution)
	);

	m_texts["ECOSYSTEM NAME"]->setFillColor(sf::Color(225, 225, 225));
	
	m_texts["ECOSYSTEM NAME"]->setOrigin(
		sf::Vector2f(
			m_texts["ECOSYSTEM NAME"]->getGlobalBounds().width / 2.0f,
			0.0f
		)
	);

	m_texts["ECOSYSTEM NAME"]->setPosition(
		sf::Vector2f(
			resolution.width / 2.0f,
			gui::p2pY(37.0f, resolution)
		)
	);
}

void LoadingState::initTextBox()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_textBox = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(37.0f, resolution),
			gui::p2pY(45.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(26.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"",
		gui::calcCharSize(26.0f, resolution),
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
		gui::p2pY(100.0f / 1080.f, resolution), 
		0.5f
	);
}

void LoadingState::initButtons()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	float positionYPercentage = 90.0f;

	m_buttons["LOAD"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(71.0f, resolution),
			gui::p2pY(positionYPercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"LOAD", 
		gui::calcCharSize(26.0f, resolution),
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

	m_buttons["OK"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(84.0f, resolution),
			gui::p2pY(positionYPercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"OK", 
		gui::calcCharSize(26.0f, resolution),
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

void LoadingState::updateInput()
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

void LoadingState::getUpdateFromButtons()
{
	if (m_buttons["LOAD"]->isClicked())
	{
		std::string folderPath = "ecosystems/" + m_textBox->getInput();

		if (m_stateData->m_ecosystem)
		{
			m_stateData->m_ecosystem->loadFromFolder(folderPath.c_str());
		}
		else
		{
			m_stateData->m_ecosystem = new Ecosystem(folderPath.c_str());
		}
	}
	else if (m_buttons["OK"]->isClicked())
	{
		endState();
	}
}
