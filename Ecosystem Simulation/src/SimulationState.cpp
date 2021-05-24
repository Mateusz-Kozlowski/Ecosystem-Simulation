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

	this->updateSideMenu();

	this->getUpdatesFromSideMenuGui();
	
	if (!this->paused)
		this->stateData->ecosystem->update(dt, *this->stateData->events, this->mousePosView, this->paused);
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
	// temporary variable:
	const std::string& guiPath = "resources/textures/GUI";

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
		gui::calcCharSize(resolution, 26.f),
		this->fonts["CONSOLAB"],
		"PLAY/STOP:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addTextureButton(
		"PAUSE",
		{
			{"PLAY IDLE", guiPath + "/play and stop/play idle.png"}, 
			{"STOP IDLE", guiPath + "/play and stop/stop idle.png"},
			
			{"PLAY HOVERED", guiPath + "/play and stop/play hovered.png"}, 
			{"STOP HOVERED", guiPath + "/play and stop/stop hovered.png"},
			
			{"PLAY PRESSED", guiPath + "/play and stop/play pressed.png"}, 
			{"STOP PRESSED", guiPath + "/play and stop/stop pressed.png"}
		},
		"PLAY IDLE",
		sf::Vector2f(
			gui::p2pX(10.33f, resolution),
			gui::p2pY(9.5f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addCenteredText(
		gui::p2pY(20.f, resolution),
		gui::calcCharSize(resolution, 26.f),
		this->fonts["CONSOLAB"],
		"SPEED:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addScaleSlider(
		"SPEED",
		sf::Vector2f(
			gui::p2pX(12.f, resolution),
			gui::p2pY(27.f, resolution)
		), 
		256.f / 1840.f,
		{ 0.0f, 1.0f },
		1.0f,
		guiPath + "/scale sliders/axes/axis idle.png",    
		guiPath + "/scale sliders/handles/handle idle.png",
		
		guiPath + "/scale sliders/axes/axis hovered.png", 
		guiPath + "/scale sliders/handles/handle hovered.png",
		
		guiPath + "/scale sliders/axes/axis pressed.png", 
		guiPath + "/scale sliders/handles/handle pressed.png"
	);

	this->sideMenu->addCenteredText(
		gui::p2pY(37.f, resolution),
		gui::calcCharSize(resolution, 26.f),
		this->fonts["CONSOLAB"],
		"MOVE THIS PANEL:",
		sf::Color(225, 225, 225)
	);
	this->sideMenu->addTextureButton(
		"ARROW",
		{ 
			{"LEFT IDLE", guiPath + "/arrows/left arrow idle.png"}, 
			{"RIGHT IDLE", guiPath + "/arrows/right arrow idle.png"}, 
			
			{"LEFT HOVERED", guiPath + "/arrows/left arrow hovered.png"}, 
			{"RIGHT HOVERED", guiPath + "/arrows/right arrow hovered.png"},
			
			{"LEFT PRESSED", guiPath + "/arrows/left arrow pressed.png"}, 
			{"RIGHT PRESSED", guiPath + "/arrows/right arrow pressed.png"} 
		},
		"RIGHT IDLE",
		sf::Vector2f(
			gui::p2pX(10.33f, resolution),
			gui::p2pY(42.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addCenteredText(
		gui::p2pY(52.f, resolution),
		gui::calcCharSize(resolution, 26.f),
		this->fonts["CONSOLAB"],
		"GOD TOOLS:",
		sf::Color(225, 225, 225)
	);

	this->sideMenu->addTextureButton(
		"CURSOR",
		{
			{"IDLE", guiPath + "/God tools/cursors/cursor idle.png"},
			{"HOVERED", guiPath + "/God tools/cursors/cursor hovered.png"},
			{"PRESSED", guiPath + "/God tools/cursors/cursor pressed.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(4.f, resolution), 
			gui::p2pY(57.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution), 
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addTextureButton(
		"REMOVE",
		{
			{"IDLE", guiPath + "/God tools/remove/remove idle.png"},
			{"HOVERED", guiPath + "/God tools/remove/remove hovered.png"},
			{"PRESSED", guiPath + "/God tools/remove/remove pressed.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(8.f, resolution),
			gui::p2pY(57.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addTextureButton(
		"MOVE",
		{
			{"IDLE", guiPath + "/God tools/replace/replace idle.png"},
			{"HOVERED", guiPath + "/God tools/replace/replace hovered.png"},
			{"PRESSED", guiPath + "/God tools/replace/replace pressed.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(12.f, resolution),
			gui::p2pY(57.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addTextureButton(
		"BRAIN",
		{
			{"IDLE", guiPath + "/God tools/brain/brain idle.png"},
			{"HOVERED", guiPath + "/God tools/brain/brain hovered.png"},
			{"PRESSED", guiPath + "/God tools/brain/brain pressed.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(16.f, resolution),
			gui::p2pY(57.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addTextureButton(
		"CLONE",
		{
			{"IDLE", guiPath + "/God tools/clone/clone idle.png"},
			{"HOVERED", guiPath + "/God tools/clone/clone hovered.png"},
			{"PRESSED", guiPath + "/God tools/clone/clone pressed.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(8.f, resolution),
			gui::p2pY(57.f, resolution) + gui::p2pX(4.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addTextureButton(
		"STOP",
		{
			{"IDLE", guiPath + "/God tools/stop/stop idle.png"},
			{"HOVERED", guiPath + "/God tools/stop/stop hovered.png"},
			{"PRESSED", guiPath + "/God tools/stop/stop pressed.png"}
		},
		"IDLE",
		sf::Vector2f(
			gui::p2pX(12.f, resolution),
			gui::p2pY(57.f, resolution) + gui::p2pX(4.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(100.f * 64.f / 1920.f, resolution),
			gui::p2pY(100.f * 64.f / 1080.f, resolution)
		)
	);

	this->sideMenu->addButton(
		"QUIT",
		sf::Vector2f(
			gui::p2pX(3.f, resolution), 
			gui::p2pY(87.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(18.f, resolution), 
			gui::p2pY(3.f * 7.f / 4.f, resolution)
		),
		gui::calcCharSize(resolution, 28U),
		this->fonts["CONSOLAB"],
		"QUIT",
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.6f, resolution)
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

void SimulationState::updateSideMenu()
{
	this->sideMenu->update(this->mousePosWindow, *this->stateData->events);

	this->updateSideMenuGui();
}

void SimulationState::getUpdatesFromSideMenuGui()
{	
	// get update from side menu texture buttons:
	if (this->sideMenu->getTextureButtons().at("PAUSE")->hasBeenClicked())
	{
		this->paused = !this->paused;

		std::string currentTextureKey = this->sideMenu->getTextureButtons().at("PAUSE")->getCurrentTextureKey();

		if (currentTextureKey.substr(0, 4) == "PLAY")
			this->sideMenu->setTextureOfTextureButton("PAUSE", currentTextureKey.replace(0, 4, "STOP"));

		else
			this->sideMenu->setTextureOfTextureButton("PAUSE", currentTextureKey.replace(0, 4, "PLAY"));
	}

	if (this->sideMenu->getTextureButtons().at("ARROW")->hasBeenClicked())
	{
		std::string currentTextureKey = this->sideMenu->getTextureButtons().at("ARROW")->getCurrentTextureKey();

		if (currentTextureKey.substr(0, 5) == "RIGHT")
		{
			this->sideMenu->setPosition(
				sf::Vector2f(
					this->stateData->gfxSettings->resolution.width - this->sideMenu->getSize().x,
					0.f
				)
			);

			this->sideMenu->setTextureOfTextureButton("ARROW", currentTextureKey.replace(0, 5, "LEFT"));
		}
		else
		{
			this->sideMenu->setPosition(
				sf::Vector2f(
					0.f,
					0.f
				)
			);

			this->sideMenu->setTextureOfTextureButton("ARROW", currentTextureKey.replace(0, 4, "RIGHT"));
		}
	}

	// get update from side menu buttons:
	if (this->sideMenu->getButtons().at("QUIT")->isClicked()) this->endState();
}

// private utilities:
void SimulationState::updateSideMenuGui()
{
	// change themes of texture buttons:
	for (auto& textureButton : this->sideMenu->getTextureButtons())
	{
		if (textureButton.first == "PAUSE") // pause button:
		{
			if (textureButton.second->getCurrentTextureKey().substr(0, 4) == "PLAY")
			{
				if (textureButton.second->isPressed())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "PLAY PRESSED");

				else if (textureButton.second->isHovered())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "PLAY HOVERED");

				else
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "PLAY IDLE");
			}
			else
			{
				if (textureButton.second->isPressed())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "STOP PRESSED");

				else if (textureButton.second->isHovered())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "STOP HOVERED");

				else
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "STOP IDLE");
			}
		}
		else if (textureButton.first == "ARROW") // arrow button:
		{
			if (textureButton.second->getCurrentTextureKey().substr(0, 5) == "RIGHT")
			{
				if (textureButton.second->isPressed())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "RIGHT PRESSED");

				else if (textureButton.second->isHovered())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "RIGHT HOVERED");

				else
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "RIGHT IDLE");
			}
			else
			{
				if (textureButton.second->isPressed())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "LEFT PRESSED");

				else if (textureButton.second->isHovered())
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "LEFT HOVERED");

				else
					this->sideMenu->setTextureOfTextureButton(textureButton.first, "LEFT IDLE");
			}
		}
		else // God tools buttons:
		{
			if (textureButton.second->isPressed())
				this->sideMenu->setTextureOfTextureButton(textureButton.first, "PRESSED");

			else if (textureButton.second->isHovered())
				this->sideMenu->setTextureOfTextureButton(textureButton.first, "HOVERED");

			else
				this->sideMenu->setTextureOfTextureButton(textureButton.first, "IDLE");
		}
	}
}
