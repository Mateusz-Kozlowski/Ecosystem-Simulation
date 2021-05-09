#include "stdafx.h"
#include "SimulationState.h"

// constructor/destructor:
SimulationState::SimulationState(StateData* state_data) : State(state_data)
{
	this->initKeybinds();
	this->initVariables();
	this->initFonts();
	this->initEcosystem();
	this->initView();
	this->initDeferredRender();
	this->initPauseMenu();
}

SimulationState::~SimulationState()
{
	delete this->pauseMenu;
}

// mutators:
void SimulationState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

void SimulationState::update(float dt)
{
	this->updateInput();

	if (!this->paused)
	{
		this->updateView();
		this->updateMousePositions(&this->view);
		this->stateData->ecosystem->update(dt);
	}
	else
	{
		this->updateMousePositions(&this->view);
		this->pauseMenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target) target = this->stateData->window;
	
	this->renderTexture.clear();

	// draw ecosystem:
	this->renderTexture.setView(this->view);

	this->stateData->ecosystem->render(this->renderTexture);

	// render pause menu:
	this->renderTexture.setView(this->renderTexture.getDefaultView());

	if (this->paused) this->pauseMenu->render(this->renderTexture);

	// final render:
	this->renderTexture.display();

	target->draw(this->renderSprite);
}

// initialization:
void SimulationState::initKeybinds()
{
	std::ifstream ifs("config/simulation_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) this->keybinds[key] = this->stateData->supportedKeys->at(key2);
	}
	else throw("ERROR::SIMULATIONSTATE::CANNOT OPEN FILE: config/simulation_keybinds.ini\n");

	ifs.close();
}

void SimulationState::initVariables()
{
	this->pauseMenu = nullptr;
	this->paused = false;
}

void SimulationState::initFonts()
{
	if (!this->font.loadFromFile("resources/fonts/Retroica.ttf")) throw("ERROR::SIMULATIONSTATE::COULD NOT LOAD FONT");
}

void SimulationState::initEcosystem()
{
	
}

void SimulationState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
		)
	);

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
			static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f
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

void SimulationState::initPauseMenu()
{
	const sf::VideoMode& videoMode = this->stateData->gfxSettings->resolution;

	this->pauseMenu = new gui::PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pauseMenu->addText(gui::p2pY(10.f, videoMode), gui::calcCharSize(videoMode, 32), "PAUSED", sf::Color::White);

	this->pauseMenu->addButton(
		"CONTINUE", gui::p2pY(74.f, videoMode), gui::p2pX(14.f, videoMode), gui::p2pY(6.f, videoMode),
		gui::calcCharSize(videoMode, 32), "CONTINUE",
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(200, 200, 200, 255), sf::Color(255, 255, 255, 255), sf::Color(100, 100, 100, 100)
	);

	this->pauseMenu->addButton(
		"QUIT", gui::p2pY(84.f, videoMode), gui::p2pX(14.f, videoMode), gui::p2pY(6.f, videoMode),
		gui::calcCharSize(videoMode, 32), "QUIT",
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(200, 200, 200, 255), sf::Color(255, 255, 255, 255), sf::Color(100, 100, 100, 100)
	);
}

// other private methods:
void SimulationState::updateInput()
{
	for (const auto& event : *this->stateData->events)
	{
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key(this->keybinds.at("CLOSE")))
		{
			this->paused = !this->paused;
			break;
		}
	}	
}

void SimulationState::updateView()
{
	// zoom view:
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) this->view.zoom(0.9f);

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) this->view.zoom(1.0f / 0.9f);

	// move view:
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	if (this->mousePosWindow.x < gui::p2pX(10.f, vm)) 
		this->view.move(-32.f * this->view.getSize().x / static_cast<float>(vm.width), 0.f);
	
	if (this->mousePosWindow.x > gui::p2pX(90.f, vm))
		this->view.move( 32.f * this->view.getSize().x / static_cast<float>(vm.width), 0.f);
	
	if (this->mousePosWindow.y < gui::p2pY(10.f, vm))
		this->view.move(0.f, -32.f * this->view.getSize().y / static_cast<float>(vm.height));
	
	if (this->mousePosWindow.y > gui::p2pY(90.f, vm))
		this->view.move(0.f,  32.f * this->view.getSize().y / static_cast<float>(vm.height));
	
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

void SimulationState::updatePauseMenuButtons()
{	
	if (this->pauseMenu->isButtonClicked("CONTINUE")) this->paused = false;
	
	else if (this->pauseMenu->isButtonClicked("QUIT")) this->endState();
}
