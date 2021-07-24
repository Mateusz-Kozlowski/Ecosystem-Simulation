#include "pch.h"
#include "App.h"

App::App()
{
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initEcosystem();
	this->initStateData();
	this->initStates();
}

App::~App()
{
	delete this->window;
	delete this->ecosystem;

	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

void App::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}

// private methods:

// initialization:
void App::initVariables()
{
	this->window = nullptr;
	this->ecosystem = nullptr;
	this->dt = 0.f;
}

void App::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("config/graphics.ini");
}

void App::initWindow()
{
	if (this->gfxSettings.fullscreen)
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Fullscreen,
			this->gfxSettings.contextSettings
		);
	else
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close,
			this->gfxSettings.contextSettings
		);

	this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);

	this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);

	this->window->setPosition(
		sf::Vector2i(
			this->gfxSettings.position.first,
			this->gfxSettings.position.second
		)
	);

	// TODO: what about that?:
	//this->window->setKeyRepeatEnabled(true);
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
			this->supportedKeys[key] = key_value;
	}
	else throw("ERROR::App::initKeys::CANNOT OPEN: " + std::string(path) + '\n');

	ifs.close();
}

void App::initEcosystem()
{
	this->ecosystem = nullptr;
}

void App::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.ecosystem = this->ecosystem;
	this->stateData.events = &this->events;
}

void App::initStates()
{
	this->states.push(new MainMenuState(&this->stateData));
}

// other private methods:
void App::updateDt()
{
	this->dt = this->clock.restart().asSeconds();
}

void App::update()
{
	this->updateEvents();

	if (!this->states.empty())
	{
		if (this->window->hasFocus())
		{
			if (this->states.top()->getQuit())
			{
				this->states.top()->endState();
				delete this->states.top();
				this->states.pop();

				if (!this->states.empty())
					this->states.top()->freeze();
				else 
					this->window->close();
			}

			if (!this->states.empty())
				this->states.top()->update(this->dt);
		}
	}
	else 
		this->window->close();
}

void App::updateEvents()
{
	this->events.clear();

	while (this->window->pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed) 
			this->window->close();

		this->events.push_back(this->event);
	}
}

void App::render()
{
	this->window->clear();

	if (!this->states.empty()) 
		this->states.top()->render();

	this->window->display();
}
