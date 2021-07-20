#include "stdafx.h"
#include "LoadingState.h"

LoadingState::LoadingState(StateData* state_data)
	: State(state_data)
{
	this->initKeybinds();
	this->initBackground();
	this->initFonts();
	this->initInputField();
	this->initButtons();
}

// public methods:
void LoadingState::update(float dt)
{
	this->updateMousePositions();

	this->updateInput();

	this->inputField->update(dt, *this->stateData->events, this->mousePosWindow);

	for (auto& button : this->buttons)
		button.second->update(this->mousePosWindow);

	this->getUpdateFromButtons();
}

void LoadingState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->stateData->window;

	target->draw(this->background);

	this->inputField->render(*target);

	for (const auto& button : this->buttons)
		button.second->render(*target);
}

// mutators:
void LoadingState::freeze()
{
	std::cout << "FREEZING IS NOT DEFINED YET!\n";

	for (auto& button : this->buttons) 
		button.second->setClickBlockade(true);
}

// initialization:
void LoadingState::initKeybinds()
{
	const char* path = "config/ecosystem_creator_keybinds.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) 
			this->keybinds[key] = this->stateData->supportedKeys->at(key2);
	}
	else throw("ERROR::ECOSYSTEMCREATORSTATE::COULD NOT OPEN: " + std::string(path));

	ifs.close();
}

void LoadingState::initBackground()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(resolution.width),
			static_cast<float>(resolution.height)
		)
	);

	this->background.setFillColor(sf::Color(28, 28, 28));
}

void LoadingState::initFonts()
{
	if (!this->font.loadFromFile("resources/fonts/consolab.ttf"))
		throw("ERROR::EcosystemCreatorState::CANNOT LOAD A FONT!\n");
}

void LoadingState::initInputField()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->inputField = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(27.f, resolution),
			gui::p2pY(37.9f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(50.f, resolution),
			gui::p2pY(7.f, resolution)
		),
		this->font, "ecosystems/big", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution), gui::p2pY(100.f / 1080.f, resolution), 0.5f
	);
}

void LoadingState::initButtons()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->buttons["LOAD"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(27.f, resolution),
			gui::p2pY(54.3f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(7.f, resolution)
		),
		this->font, "LOAD", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution), 0
	);

	this->buttons["OK"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(53.f, resolution),
			gui::p2pY(54.3f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(7.f, resolution)
		),
		this->font, "OK", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution), 1
	);
}

// other private methods:
void LoadingState::updateInput()
{
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"])) && !this->keysBlockades["CLOSE"])
		this->endState();
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"]))) 
		this->endState();
}

void LoadingState::getUpdateFromButtons()
{
	if (this->buttons["LOAD"]->isClicked())
	{
		if (this->stateData->ecosystem)
			this->stateData->ecosystem->loadFromFolder(this->inputField->getInput());
		else
			this->stateData->ecosystem = new Ecosystem(this->inputField->getInput());
	}

	else if (this->buttons["OK"]->isClicked())
		this->endState();
}
