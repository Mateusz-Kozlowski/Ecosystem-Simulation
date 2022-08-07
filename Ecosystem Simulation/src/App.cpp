#include "App.h"

App::App()
	: m_gfxSettings()
	, m_window()
	, m_supportedKeys()
	, m_states()
	, m_ecosystem()
	, m_events()
	, m_stateData()
	, m_clock()
	, m_dt(0.0f)
	, m_font()
{
	initGraphicsSettings();
	initWindow();
	initKeys();
	initStateData();
	initStates();
	initFont();
	initFPSpreview();
}

App::~App()
{
	while (!m_states.empty())
	{
		m_states.pop();
	}
}

void App::run()
{
	while (m_window.isOpen())
	{
		updateDt();
		update();
		render();
	}
}

// private methods:

// initialization:

void App::initGraphicsSettings()
{
	m_gfxSettings.loadFromFile("config/graphics.ini");
}

void App::initWindow()
{
	if (m_gfxSettings.fullscreen)
	{
		m_window.create(
			m_gfxSettings.resolution,
			m_gfxSettings.title,
			sf::Style::Fullscreen,
			m_gfxSettings.contextSettings
		);
	}
	else
	{
		m_window.create(
			m_gfxSettings.resolution,
			m_gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close,
			m_gfxSettings.contextSettings
		);
	}

	m_window.setFramerateLimit(m_gfxSettings.frameRateLimit);
	m_window.setVerticalSyncEnabled(m_gfxSettings.verticalSync);

	m_window.setPosition(
		sf::Vector2i(
			m_gfxSettings.position.first,
			m_gfxSettings.position.second
		)
	);

	// TODO: what about that?:
	// m_window->setKeyRepeatEnabled(true);
	// TODO: and maybe there are other public methods from window
	// TODO: class that should be used?
	// TODO: should some of them be read from a file?
}

void App::initKeys()
{
	const char* path = "Config/supported_keys.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value)
		{
			m_supportedKeys[key] = key_value;
		}
	}
	else
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::App::initKeys()::"
			<< "cannot open "
			<< path << '\n'
		);
	}

	ifs.close();
}

void App::initStateData()
{
	m_stateData.m_window = &m_window;
	m_stateData.m_gfxSettings = &m_gfxSettings;
	m_stateData.m_supportedKeys = &m_supportedKeys;
	m_stateData.m_states = &m_states;
	m_stateData.m_ecosystem = &m_ecosystem;
	m_stateData.m_events = &m_events;
}

void App::initStates()
{
	m_states.push(std::make_unique<MainMenuState>(&m_stateData));
}

void App::initFont()
{
	const char* filePath = "Resources/fonts/CONSOLAB.ttf";

	if (!m_font.loadFromFile(filePath))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::App::initFont()::"
			<< "Cannot load "
			<< filePath << '\n'
		);
	}
}

void App::initFPSpreview()
{
	const sf::VideoMode& resolution = m_gfxSettings.resolution;

	gui::FPSpreview::init(
		0.2f,
		sf::Color::Transparent,
		gui::calcCharSize(32.0f, resolution),
		m_font,
		sf::Color::White,
		resolution
	);
}

// utils:

void App::updateDt()
{
	m_dt = std::min(m_clock.restart().asSeconds(), 1.0f); // TODO: unhardcode
}

void App::update()
{
	updateFPSpreview();
	updateEvents();
	updateStates();
}

void App::updateFPSpreview()
{
	gui::FPSpreview::update(m_dt);
}

void App::updateEvents()
{
	m_events.clear();

	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}

		m_events.push_back(event);
	}
}

void App::updateStates()
{
	if (!m_states.empty())
	{
		if (m_window.hasFocus())
		{
			if (m_states.top()->getQuit())
			{
				m_states.top()->endState();
				m_states.pop();

				if (!m_states.empty())
				{
					m_states.top()->freeze();
				}
				else
				{
					m_window.close();
				}
			}
			
			if (!m_states.empty())
			{
				m_states.top()->update(m_dt);
			}
		}
	}
	else
	{
		m_window.close();
	}
}

void App::render()
{
	m_window.clear();

	if (!m_states.empty())
	{
		m_states.top()->render();
	}

	gui::FPSpreview::render(m_window);

	m_window.display();
}
