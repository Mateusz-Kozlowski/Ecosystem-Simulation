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

// mutators:
void MainMenuState::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

// other public methods:
void MainMenuState::update(float dt)
{
	this->updateMousePositions();	
	this->updateInput();
	this->getUpdateFromButtons();
	this->updateEcosystemText(dt);
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target) 
		target = this->stateData->window;

	target->draw(this->backgroundRect);

	this->renderButtons(*target);
	
	target->draw(this->ecosystemText);
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
	const char* path = "config/main_menu_keybinds.ini";

	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
			this->keybinds[key] = this->stateData->supportedKeys->at(key2);
	}
	else
		throw("ERROR::MAINMENUSTATE::COULD NOT OPEN: " + std::string(path));

	ifs.close();
}

void MainMenuState::initBackground()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->backgroundRect.setSize(
		sf::Vector2f
		(
			static_cast<float>(resolution.width),
			static_cast<float>(resolution.height)
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
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->buttons["SIMULATE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(19.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(7.f, resolution)
		),
		this->fonts["Retroica"], "SIMULATE", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	this->buttons["NEW ECOSYSTEM"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(31.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(7.f, resolution)
		),
		this->fonts["Retroica"], "NEW ECOSYSTEM", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	this->buttons["EDIT"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(43.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(7.f, resolution)
		),
		this->fonts["Retroica"], "EDIT", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	this->buttons["SAVE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(55.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(7.f, resolution)
		),
		this->fonts["Retroica"], "SAVE", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	this->buttons["LOAD"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(67.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(7.f, resolution)
		),
		this->fonts["Retroica"], "LOAD", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);

	this->buttons["QUIT"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(38.f, resolution), 
			gui::p2pY(79.f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(24.f, resolution), 
			gui::p2pY(7.f, resolution)
		),
		this->fonts["Retroica"], "QUIT", gui::calcCharSize(32.0f, resolution),
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.8f, resolution)
	);
}

void MainMenuState::initEcosystemText()
{
	this->ecosystemText.setFont(this->fonts["CONSOLAB"]);
	this->ecosystemText.setString("NO STRING HAS BEEN SET FOR THIS TEXT");
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"])))
		this->endState();
}

void MainMenuState::getUpdateFromButtons()
{
	for (auto& it : this->buttons)
		it.second->update(this->mousePosWindow);

	if (this->buttons["SIMULATE"]->isClicked())
	{
		// TODO: What about this?:
		/*
		if (this->stateData->ecosystem->isInitialized())
		{
			this->stateData->states->push(new SimulationState(this->stateData));
			this->stateData->states->top()->freeze();
		}
		else
			this->highlightEcosystemText();
		*/
		this->stateData->states->push(new SimulationState(this->stateData));
		this->stateData->states->top()->freeze();
	}

	else if (this->buttons["NEW ECOSYSTEM"]->isClicked())
	{
		this->stateData->states->push(new EcosystemCreatorState(this->stateData));
		this->stateData->states->top()->freeze();
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
	// TODO: ecosystem text is not updated at all! Change that!:
	/*
	if (!this->stateData->ecosystem->isInitialized())
	{
		this->ecosystemText.setString("CREATE A NEW ECOSYSTEM OR LOAD AN EXISTING ONE");

		if (this->ecosystemText.getFillColor() == this->highlightedEcosystemTextColor)
		{
			if (this->ecosystemTextStopwatch > this->highlightningTime)
				this->ecosystemText.setFillColor(this->defaultEcosystemTextColor);

			this->ecosystemTextStopwatch += dt;
		}
	}

	else 
		this->ecosystemText.setString("CURRENT ECOSYSTEM FOLDER: " + this->stateData->ecosystem->getDirectoryPath());
	*/
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
		it.second->render(target);
}
