#include "stdafx.h"
#include "MainMenuState.h"

// constructor/destructor:
MainMenuState::MainMenuState(StateData* state_data) : State(state_data)
{
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
	this->updateButtons();
	this->updateEcosystemText();
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target) target = this->stateData->window;

	target->draw(this->backgroundRect);

	this->renderButtons(*target);
	this->renderEcosystemText(*target);
}

// initialization:
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
	if (!this->font.loadFromFile("resources/fonts/Retroica.ttf")) throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	
	if (!this->font2.loadFromFile("resources/fonts/CONSOLAB.ttf")) throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
}

void MainMenuState::initButtons()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->buttons["SIMULATE"] = new gui::Button(
		gui::p2pX(38.f, vm), gui::p2pY(19.f, vm),
		gui::p2pX(24.f, vm), gui::p2pY(7.f, vm),
		this->font, "SIMULATE", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 0
	);

	this->buttons["NEW ECOSYSTEM"] = new gui::Button(
		gui::p2pX(38.f, vm), gui::p2pY(31.f, vm),
		gui::p2pX(24.f, vm), gui::p2pY(7.f, vm),
		this->font, "NEW ECOSYSTEM", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 1
	);

	this->buttons["EDIT"] = new gui::Button(
		gui::p2pX(38.f, vm), gui::p2pY(43.f, vm),
		gui::p2pX(24.f, vm), gui::p2pY(7.f, vm),
		this->font, "EDIT", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 1
	);

	this->buttons["SAVE"] = new gui::Button(
		gui::p2pX(38.f, vm), gui::p2pY(55.f, vm),
		gui::p2pX(24.f, vm), gui::p2pY(7.f, vm),
		this->font, "SAVE", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 3
	);

	this->buttons["LOAD"] = new gui::Button(
		gui::p2pX(38.f, vm), gui::p2pY(67.f, vm),
		gui::p2pX(24.f, vm), gui::p2pY(7.f, vm),
		this->font, "LOAD", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);

	this->buttons["QUIT"] = new gui::Button(
		gui::p2pX(38.f, vm), gui::p2pY(79.f, vm),
		gui::p2pX(24.f, vm), gui::p2pY(7.f, vm),
		this->font, "QUIT", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);
}

void MainMenuState::initEcosystemText()
{
	this->ecosystemText.setFont(this->font2);
	this->ecosystemText.setString("No string has been set for this text");
	this->ecosystemText.setPosition(50.0f, 50.0f);
	this->ecosystemText.setCharacterSize(32U);
	this->ecosystemText.setFillColor(sf::Color(216, 216, 216));
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

void MainMenuState::updateButtons()
{
	for (auto& it : this->buttons) it.second->update(this->mousePosWindow);

	if (this->buttons["SIMULATE"]->isClicked() && this->stateData->ecosystem)
		this->stateData->states->push(new SimulationState(this->stateData));

	else if (this->buttons["NEW ECOSYSTEM"]->isClicked())
		this->stateData->states->push(new EcosystemCreatorState(this->stateData));

	/*
	else if (this->buttons["SAVE"]->isClicked())
		this->stateData->states->push(new EcosystemCreator(this->stateData));

	else if (this->buttons["LOAD"]->isClicked())
		this->stateData->states->push(new EcosystemCreator(this->stateData));

	else if (this->buttons["EDIT"]->isClicked())
		this->stateData->states->push(new EcosystemCreator(this->stateData));
	*/

	else if (this->buttons["QUIT"]->isClicked()) 
		this->endState();
}

void MainMenuState::updateEcosystemText()
{
	if (!this->stateData->ecosystem) this->ecosystemText.setString("No ecosystem has been selected");

	else this->ecosystemText.setString("Current ecosystem name: " + this->stateData->ecosystem->getName());
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons) it.second->render(*this->stateData->window);
}

void MainMenuState::renderEcosystemText(sf::RenderTarget& target)
{
	this->stateData->window->draw(this->ecosystemText);
}
