#include "pch.h"
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
{
	initVariables();
	initGraphicsSettings();
	initWindow();
	initKeys();
	initEcosystem();
	initStateData();
	initStates();
}

App::~App()
{
	delete m_window;
	delete m_ecosystem;

	while (!m_states.empty())
	{
		delete m_states.top();
		m_states.pop();
	}
}

void App::run()
{
	while (m_window->isOpen())
	{
		updateDt();
		update();
		render();
	}
}

// private methods:

void App::initVariables()
{
	m_window = nullptr;
	m_ecosystem = nullptr;
	m_dt = 0.0f;
}

void App::initGraphicsSettings()
{
	m_gfxSettings.loadFromFile("config/graphics.ini");
}

void App::initWindow()
{
	if (m_gfxSettings.fullscreen)
	{
		m_window = new sf::RenderWindow(
			m_gfxSettings.resolution,
			m_gfxSettings.title,
			sf::Style::Fullscreen,
			m_gfxSettings.contextSettings
		);
	}
	else
	{
		m_window = new sf::RenderWindow(
			m_gfxSettings.resolution,
			m_gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close,
			m_gfxSettings.contextSettings
		);
	}

	m_window->setFramerateLimit(m_gfxSettings.frameRateLimit);
	m_window->setVerticalSyncEnabled(m_gfxSettings.verticalSync);

	m_window->setPosition(
		sf::Vector2i(
			m_gfxSettings.position.first,
			m_gfxSettings.position.second
		)
	);

	// TODO: what about that?:
	// m_window->setKeyRepeatEnabled(true);
	// TODO: and maybe there are other public methods from window
	// TODO: class that should be used?
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

void App::initEcosystem()
{
	m_ecosystem = nullptr;
}

void App::initStateData()
{
	m_stateData.m_window = m_window;
	m_stateData.m_gfxSettings = &m_gfxSettings;
	m_stateData.m_supportedKeys = &m_supportedKeys;
	m_stateData.m_states = &m_states;
	m_stateData.m_ecosystem = m_ecosystem;
	m_stateData.m_events = &m_events;
}

void App::initStates()
{
	m_states.push(new MainMenuState(&m_stateData));
}

void App::updateDt()
{
	m_dt = m_clock.restart().asSeconds();
}

void App::update()
{
	updateEvents();

	if (!m_states.empty())
	{
		if (m_window->hasFocus())
		{
			if (m_states.top()->getQuit())
			{
				m_states.top()->endState();
				delete m_states.top();
				m_states.pop();

				if (!m_states.empty())
				{
					m_states.top()->freeze();
				}
				else
				{
					m_window->close();
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
		m_window->close();
	}
}

void App::updateEvents()
{
	m_events.clear();

	sf::Event event;

	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}

		m_events.push_back(event);
	}
}

void App::render()
{
	m_window->clear();

	if (!m_states.empty())
	{
		m_states.top()->render();
	}

	m_window->display();
}
