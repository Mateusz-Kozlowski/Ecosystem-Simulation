#include "stdafx.h"
#include "SimulationState.h"

// constructor/destructor:
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
}

SimulationState::~SimulationState()
{
	delete this->sideMenu;
}

// mutators:
void SimulationState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

void SimulationState::update(float dt)
{
	this->updateInput();

	this->updateView();

	this->updateMousePositions(&this->view);
	
	if (!this->paused)
		this->stateData->ecosystem->update(dt, *this->stateData->events, this->mousePosView, this->paused);

	this->sideMenu->update(this->mousePosWindow, *this->stateData->events);
	this->getUpdateFromSideMenuGui();
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target) target = this->stateData->window;
	
	this->renderTexture.clear();

	// draw ecosystem:
	this->renderTexture.setView(this->view);

	this->stateData->ecosystem->render(this->renderTexture);

	// render simulation menu:
	this->renderTexture.setView(this->renderTexture.getDefaultView());

	if (this->sideMenuIsRendered) this->sideMenu->render(this->renderTexture);

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
	this->sideMenu = nullptr;
	this->sideMenuIsRendered = false;
	this->paused = true;
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
	const sf::VideoMode resolution = this->stateData->gfxSettings->resolution;

	// create new SideMenu:
	this->sideMenu = new gui::SideMenu(
		sf::Vector2f(0.f, 0.f),
		sf::Vector2f(gui::p2pX(24, resolution), gui::p2pY(100, resolution)),
		sf::Color(48, 48, 48)
	);

	// add widgets:
	this->sideMenu->addCenteredText(
		gui::p2pY(4.f, resolution),
		gui::calcCharSize(resolution, 28.f),
		this->fonts["CONSOLAB"],
		"PLAY/STOP:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addTextureButton(
		"PAUSE",
		{ {"PLAY", "resources/textures/GUI/SideMenu/play.png"}, {"STOP", "resources/textures/GUI/SideMenu/stop.png"} },
		"PLAY",
		gui::p2pX(10.33f, resolution), gui::p2pY(9.5f, resolution),
		gui::p2pX(100.f * 64.f / 1920.f, resolution), gui::p2pY(100.f * 64.f / 1080.f, resolution)
	);

	this->sideMenu->addCenteredText(
		gui::p2pY(20.f, resolution),
		gui::calcCharSize(resolution, 30.f),
		this->fonts["CONSOLAB"],
		"SPEED:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addScaleSlider(
		"SPEED",
		gui::p2pX(12.f, resolution), gui::p2pY(27.f, resolution),
		256.f / 1840.f,
		{ 0.0f, 1.0f },
		1.0f,
		"resources/textures/GUI/SideMenu/axis idle.png", "resources/textures/GUI/SideMenu/handle idle.png",
		"resources/textures/GUI/SideMenu/axis hovered.png", "resources/textures/GUI/SideMenu/handle hovered.png",
		"resources/textures/GUI/SideMenu/axis pressed.png", "resources/textures/GUI/SideMenu/handle pressed.png"
	);

	this->sideMenu->addCenteredText(
		gui::p2pY(37.f, resolution),
		gui::calcCharSize(resolution, 27.f),
		this->fonts["CONSOLAB"],
		"MOVE THIS PANEL:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addTextureButton(
		"ARROW",
		{ {"LEFT", "resources/textures/GUI/SideMenu/left arrow.png"}, {"RIGHT", "resources/textures/GUI/SideMenu/right arrow.png"} },
		"RIGHT",
		gui::p2pX(10.33f, resolution), gui::p2pY(42.f, resolution),
		gui::p2pX(100.f * 64.f / 1920.f, resolution), gui::p2pY(100.f * 64.f / 1080.f, resolution)
	);

	this->sideMenu->addButton(
		"QUIT",
		sf::Vector2f(gui::p2pX(5.f, resolution), gui::p2pY(87.f, resolution)),
		gui::p2pX(14.f, resolution), gui::p2pY(6.f, resolution),
		gui::calcCharSize(resolution, 28U),
		this->fonts["CONSOLAB"],
		"QUIT",
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
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
				this->sideMenuIsRendered = !this->sideMenuIsRendered;
				break;
			}
			if (event.key.code == sf::Keyboard::Key(this->keybinds.at("PAUSE")))
			{
				this->paused = !this->paused;
				
				if (this->paused)
					this->sideMenu->setTextureOfTextureButton("PAUSE", "PLAY");
				else
					this->sideMenu->setTextureOfTextureButton("PAUSE", "STOP");

				break;
			}
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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		int offsetX = this->previousMousePosWindow.x - this->mousePosWindow.x;
		int offsetY = this->previousMousePosWindow.y - this->mousePosWindow.y;

		this->view.move(
			offsetX * this->view.getSize().x / this->stateData->gfxSettings->resolution.width,
			offsetY * this->view.getSize().y / this->stateData->gfxSettings->resolution.height
		);
	}
	
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

void SimulationState::getUpdateFromSideMenuGui()
{	
	this->getUpdateFromSideMenuButtons();
	this->getUpdateFromSideMenuTexturesButtons();
}

// private utilities:
void SimulationState::getUpdateFromSideMenuTexturesButtons()
{
	if (this->sideMenu->getTextureButtons().at("PAUSE")->hasBeenClicked())
	{
		this->paused = !this->paused;

		const std::string& currentTextureKey = this->sideMenu->getTextureButtons().at("PAUSE")->getCurrentTextureKey();

		if (currentTextureKey == "PLAY") this->sideMenu->setTextureOfTextureButton("PAUSE", "STOP");

		else this->sideMenu->setTextureOfTextureButton("PAUSE", "PLAY");
	}

	if (this->sideMenu->getTextureButtons().at("ARROW")->hasBeenClicked())
	{
		const std::string& currentTextureKey = this->sideMenu->getTextureButtons().at("ARROW")->getCurrentTextureKey();

		if (currentTextureKey == "RIGHT")
		{
			this->sideMenu->setPosition(
				sf::Vector2f(
					this->stateData->gfxSettings->resolution.width - this->sideMenu->getSize().x,
					0.f
				)
			);

			this->sideMenu->setTextureOfTextureButton("ARROW", "LEFT");
		}
		else
		{
			this->sideMenu->setPosition(
				sf::Vector2f(
					0.f,
					0.f
				)
			);

			this->sideMenu->setTextureOfTextureButton("ARROW", "RIGHT");
		}
	}
}

void SimulationState::getUpdateFromSideMenuButtons()
{
	if (this->sideMenu->getButtons().at("QUIT")->isClicked())
		this->endState();
}
