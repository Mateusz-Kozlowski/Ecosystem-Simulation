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

	/*
	std::cout << this->renderTexture.getViewport(this->view).left << ' ' << this->renderTexture.getViewport(this->view).top << '\n';
	std::cout << this->renderTexture.getViewport(this->view).width << ' ' << this->renderTexture.getViewport(this->view).height << '\n';
	*/

	sf::RectangleShape temp(sf::Vector2f(3840.f, 2160.f));
	//sf::RectangleShape temp(sf::Vector2f(1920.f, 1080.f));

	temp.setOrigin(0, 0);

	temp.setPosition(sf::Vector2f(0, 0));

	temp.setFillColor(sf::Color(200, 200, 100));

	this->renderTexture.draw(temp);

	this->stateData->ecosystem->render(this->renderTexture);

	// render GUI:
	// this->miniMap->render(this->renderTexture);
	// minimap kinda works:
	sf::View miniMapView(this->renderTexture.getDefaultView());

	miniMapView.setViewport(sf::FloatRect(0.75f, 0.75f, 0.25f, 0.25f));

	miniMapView.setSize(3840, 2160);

	this->renderTexture.setView(miniMapView);

	this->renderTexture.draw(temp);

	this->stateData->ecosystem->render(this->renderTexture);

	// pause menu:
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
	this->miniMap = new MiniMap(sf::Vector2u(3840U, 2160U), sf::Vector2u(0.25f, 0.25f), sf::Vector2u(0.75f, 0.75f));
}

void SimulationState::initPauseMenu()
{
	const sf::VideoMode& videoMode = this->stateData->gfxSettings->resolution;

	this->pauseMenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) this->paused = true;
}

void SimulationState::updateView()
{
	if (this->mousePosWindow.x < 100 && this->view.getCenter().x > 0) this->view.move(-24.f, 0.f);
	
	if (this->mousePosWindow.x > 1820 && this->view.getCenter().x < 3840) this->view.move(24.f, 0.f);
	
	if (this->mousePosWindow.y < 100 && this->view.getCenter().y > 0) this->view.move(0.f, -24.f);
	
	if (this->mousePosWindow.y > 980 && this->view.getCenter().y < 2160) this->view.move(0.f, 24.f);
	
	// zoom:
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) this->view.zoom(0.9);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) this->view.zoom(1.1);


	/*
	this->view.setCenter(
		std::floor((static_cast<float>(this->mousePosWindow.x) - static_cast<float>(this->stateData->gfxSettings->resolution.width / 2U)) / 10.f),
		std::floor((static_cast<float>(this->mousePosWindow.y) - static_cast<float>(this->stateData->gfxSettings->resolution.height / 2U)) / 10.f)
	);
	*/

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
	if (this->pauseMenu->isButtonClicked("QUIT")) this->endState();
}
