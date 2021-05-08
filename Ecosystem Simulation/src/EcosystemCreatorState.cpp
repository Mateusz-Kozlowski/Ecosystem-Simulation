#include "stdafx.h"
#include "EcosystemCreatorState.h"

// constructor/destructor:
EcosystemCreatorState::EcosystemCreatorState(StateData* state_data) : State(state_data)
{
	this->initKeybinds();
	this->initFonts();
	this->initButtons();
	this->initInputField();
}

EcosystemCreatorState::~EcosystemCreatorState()
{
	for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) delete it->second;

	delete this->inputField;
}

// mutators:
void EcosystemCreatorState::freeze()
{
	std::cout << "FREEZING IS NOT DEFINED YET!\n";
}

void EcosystemCreatorState::update(float dt)
{
	this->updateInput();

	this->inputField->update(dt, *this->stateData->events);

	this->updateButtons();
}

void EcosystemCreatorState::render(sf::RenderTarget* target)
{
	if (!target) target = this->stateData->window;

	this->renderButtons(*target);
	this->inputField->render(*target);
	this->renderButtons(*target);
}

// initialization:
void EcosystemCreatorState::initKeybinds()
{
	std::string path = "config/ecosystem_creator_keybinds.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) this->keybinds[key] = this->stateData->supportedKeys->at(key2);
	}
	else throw("ERROR::ECOSYSTEMCREATORSTATE::COULD NOT OPEN: " + path);

	ifs.close();
}

void EcosystemCreatorState::initFonts()
{
	if (!this->font.loadFromFile("resources/fonts/consolab.ttf"))
		throw("ERROR::EcosystemCreatorState::CANNOT LOAD A FONT!\n");
}

void EcosystemCreatorState::initButtons()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->buttons["TEMP"] = new gui::Button(
		gui::p2pX(45.f, vm), gui::p2pY(40.f, vm),
		gui::p2pX(10.f, vm), gui::p2pY(10.f, vm),
		this->font, "QUIT", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);
}

void EcosystemCreatorState::initInputField()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->inputField = new gui::InputField(
		gui::p2pX(45.f, vm), gui::p2pY(25.f, vm),
		gui::p2pX(10.f, vm), gui::p2pY(10.f, vm),
		this->font, "ecosystems", gui::p2pY(8.f, vm),
		sf::Color(100, 100, 100), sf::Color(64, 64, 64), sf::Color(64, 64, 64),
		gui::p2pX(0.2f, vm)
	);
}

// other private methods:
void EcosystemCreatorState::updateInput()
{

}

void EcosystemCreatorState::updateButtons()
{

}

void EcosystemCreatorState::renderButtons(sf::RenderTarget& target)
{
	this->buttons["TEMP"]->render(target);
}
