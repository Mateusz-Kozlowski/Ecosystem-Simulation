#include "pch.h"
#include "LoadingState.h"

LoadingState::LoadingState(StateData* state_data)
	: State(state_data)
{
	this->initKeybinds();
	this->initBackground();
	this->initContainer();
	this->initFonts();
	this->initTexts();
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
	target->draw(this->container);

	for (const auto& text : this->texts)
		target->draw(*text.second);

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

// private methods:

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

void LoadingState::initContainer()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->container = sf::RectangleShape(
		sf::Vector2f(
			gui::p2pX(34.0f, resolution),
			gui::p2pY(100.0f, resolution)
		)
	);
	this->container.setFillColor(sf::Color(0, 0, 0, 128));
	this->container.setPosition(
		sf::Vector2f(
			resolution.width / 2.0f - this->container.getGlobalBounds().width / 2.0f,
			0.0f
		)
	);
}

void LoadingState::initFonts()
{
	if (!this->font.loadFromFile("resources/fonts/consolab.ttf"))
		throw("ERROR::EcosystemCreatorState::CANNOT LOAD A FONT!\n");
}

void LoadingState::initTexts()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->texts["ECOSYSTEM NAME"] = std::make_unique<sf::Text>(
		"ECOSYSTEM NAME:",
		this->font,
		gui::calcCharSize(32.0f, resolution)
	);
	this->texts["ECOSYSTEM NAME"]->setFillColor(sf::Color(225, 225, 225));
	this->texts["ECOSYSTEM NAME"]->setOrigin(
		sf::Vector2f(
			this->texts["ECOSYSTEM NAME"]->getGlobalBounds().width / 2.0f,
			0.0f
		)
	);
	this->texts["ECOSYSTEM NAME"]->setPosition(
		sf::Vector2f(
			resolution.width / 2.0f,
			gui::p2pY(37.0f, resolution)
		)
	);
}

void LoadingState::initInputField()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->inputField = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(37.0f, resolution),
			gui::p2pY(45.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(26.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->font, "",
		gui::calcCharSize(26.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f / 1080.f, resolution), 0.5f
	);
}

void LoadingState::initButtons()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	float positionYPercentage = 90.0f;

	this->buttons["LOAD"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(71.0f, resolution),
			gui::p2pY(positionYPercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->font, "LOAD", gui::calcCharSize(26.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->buttons["OK"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(84.0f, resolution),
			gui::p2pY(positionYPercentage, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->font, "OK", gui::calcCharSize(26.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
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
		std::string folderPath = "ecosystems/" + this->inputField->getInput();

		if (this->stateData->ecosystem)
			this->stateData->ecosystem->loadFromFolder(folderPath);
		else
			this->stateData->ecosystem = new Ecosystem(folderPath);
	}

	else if (this->buttons["OK"]->isClicked())
		this->endState();
}
