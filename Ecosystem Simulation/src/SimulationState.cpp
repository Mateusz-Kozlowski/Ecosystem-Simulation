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
	
	if (!this->paused) this->stateData->ecosystem->update(dt, *this->stateData->events, this->mousePosView);

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
	if (!this->font.loadFromFile("resources/fonts/Retroica.ttf")) throw("ERROR::SIMULATIONSTATE::COULD NOT LOAD FONT");
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

	this->sideMenu = new gui::SideMenu(
		this->font,
		sf::Vector2f(0.f, 0.f),
		sf::Vector2f(gui::p2pX(20, resolution), gui::p2pY(100, resolution)),
		sf::Color(64, 64, 64, 200)
	);

	this->sideMenu->addTextureButton(
		"PAUSE",
		{ {"PLAY", "resources/textures/GUI/SideMenu/play.png"}, {"STOP", "resources/textures/GUI/SideMenu/stop.png"} },
		"STOP",
		gui::p2pX(4.f, resolution), gui::p2pY(4.f, resolution),
		gui::p2pX(100.f * 64.f / 1920.f, resolution), gui::p2pY(100.f * 64.f / 1080.f, resolution)
	);

	this->sideMenu->addButton(
		"QUIT",
		sf::Vector2f(gui::p2pX(4.f, resolution), gui::p2pY(75.f, resolution)),
		gui::p2pX(12.f, resolution), gui::p2pY(4.f, resolution),
		gui::calcCharSize(resolution, 32U),
		"QUIT",
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
		sf::Color::White, sf::Color(128, 128, 128), sf::Color(32, 32, 32)
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
	if (this->sideMenu->getTextureButtons().at("PAUSE")->hasBeenClicked())
	{
		this->paused = !this->paused;

		const std::string& currentTextureKey = this->sideMenu->getTextureButtons().at("PAUSE")->getCurrentTextureKey();

		if (currentTextureKey == "PLAY") this->sideMenu->setTextureOfTextureButton("PAUSE", "STOP");

		else this->sideMenu->setTextureOfTextureButton("PAUSE", "PLAY");
	}

	if (this->sideMenu->getButtons().at("QUIT")->isClicked()) this->endState();
}
