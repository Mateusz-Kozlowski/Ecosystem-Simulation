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
	this->initMiniMap();
	this->initPauseMenu();
}

SimulationState::~SimulationState()
{
	delete this->miniMap;
	delete this->pauseMenu;
}

// mutators:
void SimulationState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

void SimulationState::update(float dt)
{
	this->updateMousePositions(&this->view);
	this->updateInput();

	if (!this->paused)
	{
		this->updateView();
		this->stateData->ecosystem->update();
		this->miniMap->update();
	}
	else
	{
		this->pauseMenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->stateData->window;
	
	this->renderTexture.clear();

	this->renderTexture.setView(this->view);

	this->stateData->ecosystem->render(this->renderTexture);

	// render GUI:
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
	this->miniMap = nullptr;
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
			static_cast<float>(this->stateData->gfxSettings->resolution.width / 2U),
			static_cast<float>(this->stateData->gfxSettings->resolution.height / 2U)
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

void SimulationState::initMiniMap()
{
	//this->miniMap = new MiniMap();
}

void SimulationState::initPauseMenu()
{
	//this->pauseMenu = new PauseMenu();
}

// other private methods:
void SimulationState::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) this->paused = !this->paused;
}

void SimulationState::updateView()
{
	this->view.setCenter(
		std::floor((static_cast<float>(this->mousePosWindow.x) - static_cast<float>(this->stateData->gfxSettings->resolution.width / 2U)) / 10.f),
		std::floor((static_cast<float>(this->mousePosWindow.y) - static_cast<float>(this->stateData->gfxSettings->resolution.height / 2U)) / 10.f)
	);

	/*
	if (this->tileMap->getMaxSizeF().x >= this->view.getSize().x)
	{
		if (this->view.getCenter().x - this->view.getSize().x / 2.f < 0.f)
		{
			this->view.setCenter(0.f + this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
		else if (this->view.getCenter().x + this->view.getSize().x / 2.f > this->tileMap->getMaxSizeF().x)
		{
			this->view.setCenter(this->tileMap->getMaxSizeF().x - this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
	}

	if (this->tileMap->getMaxSizeF().y >= this->view.getSize().y)
	{
		if (this->view.getCenter().y - this->view.getSize().y / 2.f < 0.f)
		{
			this->view.setCenter(this->view.getCenter().x, 0.f + this->view.getSize().y / 2.f);
		}
		else if (this->view.getCenter().y + this->view.getSize().y / 2.f > this->tileMap->getMaxSizeF().y)
		{
			this->view.setCenter(this->view.getCenter().x, this->tileMap->getMaxSizeF().y - this->view.getSize().y / 2.f);
		}
	}
	*/

	//this->viewGridPosition.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	//this->viewGridPosition.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void SimulationState::updatePauseMenuButtons()
{
	if (this->pauseMenu->isButtonPressed("QUIT")) this->endState();
}
