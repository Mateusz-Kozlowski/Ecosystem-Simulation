#include "stdafx.h"
#include "EcosystemCreatorState.h"

// constructor/destructor:
EcosystemCreatorState::EcosystemCreatorState(StateData* state_data) : State(state_data)
{
	this->initKeybinds();
	this->initBackground();
	this->initFonts();
	this->initTexts();
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
	this->updateMousePositions();
	
	this->updateInput();

	this->inputField->update(dt, *this->stateData->events);

	this->updateButtons();
}

void EcosystemCreatorState::render(sf::RenderTarget* target)
{
	if (!target) target = this->stateData->window;

	target->draw(this->backgroundRect);

	for (const auto& text : this->texts) target->draw(text.second);
	
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

void EcosystemCreatorState::initBackground()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->backgroundRect.setSize(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);

	this->backgroundRect.setFillColor(sf::Color(28, 28, 28));
}

void EcosystemCreatorState::initFonts()
{
	if (!this->font.loadFromFile("resources/fonts/consolab.ttf"))
		throw("ERROR::EcosystemCreatorState::CANNOT LOAD A FONT!\n");
}

void EcosystemCreatorState::initTexts()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	// title:
	this->texts["TITLE"] = sf::Text();
	this->texts["TITLE"].setFont(this->font);
	this->texts["TITLE"].setString("HOW TO CREATE A NEW ECOSYSTEM?\n\n");
	this->texts["TITLE"].setCharacterSize(gui::calcCharSize(vm, 32U));
	this->texts["TITLE"].setPosition(gui::p2pX(50.f, vm), gui::p2pY(8.f, vm));
	this->texts["TITLE"].setOrigin(
		this->texts["TITLE"].getGlobalBounds().width / 2.f,
		0.f
	);
	this->texts["TITLE"].setFillColor(sf::Color::White);
	
	// instructions:
	std::stringstream instructions;
	
	instructions << "1. Enter the path of the folder that will contain a new ecosystem.\n";
	instructions << "   - The path can be relative or absolute.\n";
	instructions << "   - The ecosystem will be created directly in the specified folder,\n";
	instructions << "     so its path should contain the name of that ecosystem \n";
	instructions << "     e.g: ecosystems/ecosystem 1\n";
	instructions << "2. Click CREATE CONFIG FILE button, which will open File Explorer\n";
	instructions << "3. In File Explorer go to the specified folder\n";
	instructions << "4. The folder should contain a file named: ecosystem initializator.ini\n";
	instructions << "5. Open the file and edit its values according to your preferences\n";
	instructions << "6. Save and close the file, do not change its name\n";
	instructions << "7. Click CREATE ECOSYSTEM button\n";
	instructions << "8. If u want to load that ecosystem (u probably want to do this) click LOAD button\n";
	instructions << "9. Click QUIT button, start simulation and enjoy that beautiful view!";

	this->texts["INSTRUCTIONS"].setFont(this->font);
	this->texts["INSTRUCTIONS"].setString(instructions.str());
	this->texts["INSTRUCTIONS"].setCharacterSize(gui::calcCharSize(vm, 24U));
	this->texts["INSTRUCTIONS"].setLineSpacing(1.3f);
	this->texts["INSTRUCTIONS"].setPosition(gui::p2pX(50.f, vm), gui::p2pY(16.f, vm));
	this->texts["INSTRUCTIONS"].setOrigin(
		this->texts["INSTRUCTIONS"].getGlobalBounds().width  / 2.f,
		0.f
	);
	this->texts["INSTRUCTIONS"].setFillColor(sf::Color::White);
}

void EcosystemCreatorState::initButtons()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->buttons["CREATE CONFIG FILE"] = new gui::Button(
		gui::p2pX(11.f, vm), gui::p2pY(83.f, vm),
		gui::p2pX(18.f, vm), gui::p2pY(7.f, vm),
		this->font, "CREATE CONFIG FILE", gui::calcCharSize(vm, 30),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);

	// TODO: it will be added later:
	/*
	this->buttons["OPEN FILE EXPLORER"] = new gui::Button(
		gui::p2pX(11.f, vm), gui::p2pY(83.f, vm),
		gui::p2pX(18.f, vm), gui::p2pY(7.f, vm),
		this->font, "CREATE CONFIG FILE", gui::calcCharSize(vm, 30),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);
	*/

	this->buttons["CREATE ECOSYSTEM"] = new gui::Button(
		gui::p2pX(31.f, vm), gui::p2pY(83.f, vm),
		gui::p2pX(18.f, vm), gui::p2pY(7.f, vm),
		this->font, "CREATE ECOSYSTEM", gui::calcCharSize(vm, 30),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);

	this->buttons["LOAD"] = new gui::Button(
		gui::p2pX(51.f, vm), gui::p2pY(83.f, vm),
		gui::p2pX(18.f, vm), gui::p2pY(7.f, vm),
		this->font, "LOAD", gui::calcCharSize(vm, 32),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, vm), 4
	);

	this->buttons["QUIT"] = new gui::Button(
		gui::p2pX(71.f, vm), gui::p2pY(83.f, vm),
		gui::p2pX(18.f, vm), gui::p2pY(7.f, vm),
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
		gui::p2pX(35.f, vm), gui::p2pY(71.f, vm),
		gui::p2pX(30.f, vm), gui::p2pY(5.f, vm),
		this->font, "ecosystems/single", gui::p2pY(3.f, vm),
		sf::Color(100, 100, 100), sf::Color(255, 255, 255), sf::Color(75, 75, 75),
		gui::p2pX(0.2f, vm)
	);
}

// other private methods:
void EcosystemCreatorState::updateInput()
{

}

void EcosystemCreatorState::updateButtons()
{
	for (auto& it : this->buttons) it.second->update(this->mousePosWindow);

	if (this->buttons["CREATE CONFIG FILE"]->isClicked())
	{
		Ecosystem::createConfigFile(this->inputField->getInput());
		
		// TODO: add open File Explorer button!
		//system("explorer");
	}

	if (this->buttons["CREATE ECOSYSTEM"]->isClicked())
	{
		delete this->stateData->ecosystem;
		Ecosystem::setUpEcosystemFolder(this->inputField->getInput());
	}

	if (this->buttons["LOAD"]->isClicked())
	{
		delete this->stateData->ecosystem;
		this->stateData->ecosystem = new Ecosystem();
		this->stateData->ecosystem->loadFromFolder(this->inputField->getInput());
	}
	
	if (this->buttons["QUIT"]->isClicked()) this->endState();
}

void EcosystemCreatorState::renderButtons(sf::RenderTarget& target)
{	
	for (auto& it : this->buttons) it.second->render(target);
}
