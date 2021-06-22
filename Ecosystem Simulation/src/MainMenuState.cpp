#include "stdafx.h"
#include "MainMenuState.h"

// constructor/destructor:
MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initKeybinds();
	this->initBackground();
	this->initFonts();
	this->initButtons();
	this->initEcosystemText();
}

MainMenuState::~MainMenuState()
{
	for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) delete it->second;
}

// mutators:
void MainMenuState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

void MainMenuState::update(float dt)
{
	this->updateMousePositions();
	this->updateInput();
	this->getUpdateFromButtons();
	this->updateEcosystemText(dt);
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target) target = this->stateData->window;

	target->draw(this->backgroundRect);

	this->renderButtons(*target);
	this->renderEcosystemText(*target);
}

// initialization:
void MainMenuState::initVariables()
{
	this->defaultEcosystemTextColor = sf::Color(216, 216, 216);
	this->highlightedEcosystemTextColor = sf::Color::Red;

	this->ecosystemTextStopwatch = 0.0f;
	this->highlightningTime = 0.5f;
}

void MainMenuState::initKeybinds()
{
	std::string path = "config/main_menu_keybinds.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) this->keybinds[key] = this->stateData->supportedKeys->at(key2);
	}
	else throw("ERROR::MAINMENUSTATE::COULD NOT OPEN: " + path);

	ifs.close();
}

void MainMenuState::initBackground()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->backgroundRect.setSize(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);

	this->backgroundRect.setFillColor(sf::Color(32, 32, 32));
}

void MainMenuState::initFonts()
{
	if (!this->fonts["Retroica"].loadFromFile("resources/fonts/Retroica.ttf"))
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD A FONT");
	
	if (!this->fonts["CONSOLAB"].loadFromFile("resources/fonts/CONSOLAB.ttf"))
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD A FONT");
}

void MainMenuState::initButtons()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->buttons["SIMULATE"] = new gui::Button(
		sf::Vector2f(
			gui::p2pX(38.f, vm), 
			gui::p2pY(19.f, vm)
		),
		sf::Vector2f(
			gui::p2pX(24.f, vm), 
			gui::p2pY(7.f, vm)
		),
		this->fonts["Retroica"], "SIMULATE", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, vm)
	);

	this->buttons["NEW ECOSYSTEM"] = new gui::Button(
		sf::Vector2f(
			gui::p2pX(38.f, vm), 
			gui::p2pY(31.f, vm)
		),
		sf::Vector2f(
			gui::p2pX(24.f, vm), 
			gui::p2pY(7.f, vm)
		),
		this->fonts["Retroica"], "NEW ECOSYSTEM", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, vm)
	);

	this->buttons["EDIT"] = new gui::Button(
		sf::Vector2f(
			gui::p2pX(38.f, vm), 
			gui::p2pY(43.f, vm)
		),
		sf::Vector2f(
			gui::p2pX(24.f, vm), 
			gui::p2pY(7.f, vm)
		),
		this->fonts["Retroica"], "EDIT", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, vm)
	);

	this->buttons["SAVE"] = new gui::Button(
		sf::Vector2f(
			gui::p2pX(38.f, vm), 
			gui::p2pY(55.f, vm)
		),
		sf::Vector2f(
			gui::p2pX(24.f, vm), 
			gui::p2pY(7.f, vm)
		),
		this->fonts["Retroica"], "SAVE", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, vm)
	);

	this->buttons["LOAD"] = new gui::Button(
		sf::Vector2f(
			gui::p2pX(38.f, vm), 
			gui::p2pY(67.f, vm)
		),
		sf::Vector2f(
			gui::p2pX(24.f, vm), 
			gui::p2pY(7.f, vm)
		),
		this->fonts["Retroica"], "LOAD", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, vm)
	);

	this->buttons["QUIT"] = new gui::Button(
		sf::Vector2f(
			gui::p2pX(38.f, vm), 
			gui::p2pY(79.f, vm)
		),
		sf::Vector2f(
			gui::p2pX(24.f, vm), 
			gui::p2pY(7.f, vm)
		),
		this->fonts["Retroica"], "QUIT", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, vm)
	);
}

void MainMenuState::initEcosystemText()
{
	this->ecosystemText.setFont(this->fonts["CONSOLAB"]);
	this->ecosystemText.setString("No string has been set for this text");
	this->ecosystemText.setPosition(50.0f, 50.0f);
	this->ecosystemText.setCharacterSize(32U);
	this->ecosystemText.setFillColor(this->defaultEcosystemTextColor);
}

// private utilities:
void MainMenuState::highlightEcosystemText()
{
	this->ecosystemText.setFillColor(this->highlightedEcosystemTextColor);
	this->ecosystemTextStopwatch = 0.0f;
}

void MainMenuState::saveEcosystem(const Ecosystem& ecosystem)
{
	std::cout << "SAVE IS NOT DEFINET YET\n";
}

// other private methods:
void MainMenuState::updateInput()
{
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"])) && !this->keysBlockades["CLOSE"])
		this->endState();
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"]))) this->endState();
}

void MainMenuState::getUpdateFromButtons()
{
	for (auto& it : this->buttons) it.second->update(this->mousePosWindow);

	if (this->buttons["SIMULATE"]->isClicked())
	{
		if (this->stateData->ecosystem)
		{
			this->stateData->states->push(new SimulationState(this->stateData));
			this->stateData->states->top()->freeze();
		}
		else
			this->highlightEcosystemText();
	}

	else if (this->buttons["NEW ECOSYSTEM"]->isClicked())
	{
		//this->stateData->states->push(new EcosystemCreatorState(this->stateData));
		//this->stateData->states->top()->freeze();
		std::cout << "ECOSYSTEM CREATION STATE IS NOT DEFINED YET!\n";
	}

	else if (this->buttons["SAVE"]->isClicked())
		this->saveEcosystem(*this->stateData->ecosystem);

	else if (this->buttons["LOAD"]->isClicked())
	{
		this->stateData->states->push(new LoadingState(this->stateData));
		this->stateData->states->top()->freeze();
	}

	else if (this->buttons["EDIT"]->isClicked())
	{
		//this->stateData->states->push(new EditorState(this->stateData));
		//this->stateData->states->top()->freeze();
		std::cout << "EDIT IS NOT DEFINET\n";
	}

	else if (this->buttons["QUIT"]->isClicked()) 
		this->endState();
}

void MainMenuState::updateEcosystemText(float dt)
{
	if (!this->stateData->ecosystem)
	{
		this->ecosystemText.setString("Create a new ecosystem or load an existing one");

		if (this->ecosystemText.getFillColor() == this->highlightedEcosystemTextColor)
		{
			if (this->ecosystemTextStopwatch > this->highlightningTime)
				this->ecosystemText.setFillColor(this->defaultEcosystemTextColor);

			this->ecosystemTextStopwatch += dt;
		}
	}

	else this->ecosystemText.setString("Current ecosystem folder: " + this->stateData->ecosystem->getDirectoryPath());
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons) it.second->render(target);
}

void MainMenuState::renderEcosystemText(sf::RenderTarget& target)
{
	this->stateData->window->draw(this->ecosystemText);
}
