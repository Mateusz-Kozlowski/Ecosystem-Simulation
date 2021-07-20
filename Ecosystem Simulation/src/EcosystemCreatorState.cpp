#include "stdafx.h"
#include "EcosystemCreatorState.h"

EcosystemCreatorState::EcosystemCreatorState(StateData* state_data)
	: State(state_data)
{
	this->initKeybinds();
	this->initBackground();
	this->initFonts();
	this->initGui();
}

// public methods:
void EcosystemCreatorState::update(float dt)
{
	this->updateMousePositions();
	this->updateInput();
	this->updateGui(dt);
	this->getUpdatesFromGui();
}

void EcosystemCreatorState::render(sf::RenderTarget* target)
{
	if (!target) 
		target = this->stateData->window;

	target->draw(this->background);

	this->renderGui(*target);
}

// mutators:
void EcosystemCreatorState::freeze()
{
	std::cout << "FREEZING IS NOT DEFINED YET!\n";
}

// private methods:

// initialization:
void EcosystemCreatorState::initKeybinds()
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
	else throw("ERROR::EcosystemCreatorState::initKeybinds::COULD NOT OPEN: " + std::string(path));

	ifs.close();
}

void EcosystemCreatorState::initBackground()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(resolution.width),
			static_cast<float>(resolution.height)
		)
	);

	this->background.setFillColor(sf::Color(32, 32, 32));
}

void EcosystemCreatorState::initFonts()
{
	if (!this->fonts["RETROICA"].loadFromFile("resources/fonts/Retroica.ttf"))
		throw("ERROR::EcosystemCreatorState::initFonts::COULD NOT LOAD A FONT\n");

	if (!this->fonts["CONSOLAB"].loadFromFile("resources/fonts/consolab.ttf"))
		throw("ERROR::EcosystemCreatorState::initFonts::COULD NOT LOAD A FONT\n");
}

void EcosystemCreatorState::initGui()
{
	this->initButtons();
	this->initInputFields();
	this->initTexts();
}

void EcosystemCreatorState::initButtons()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(20.0f, resolution);

	this->buttons["SMALL"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(30.0f, resolution),
			gui::p2pY(7.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "SMALL", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->buttons["BIG"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(7.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "BIG", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->buttons["HUGE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(58.0f, resolution),
			gui::p2pY(7.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "HUGE", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->buttons["CREATE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(71.0f, resolution),
			gui::p2pY(90.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "CREATE", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	this->buttons["OK"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(84.0f, resolution),
			gui::p2pY(90.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "OK", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);
}

void EcosystemCreatorState::initInputFields()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(20.0f, resolution);

	this->inputFields["WORLD WIDTH"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(21.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "3840", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f
	);

	this->inputFields["WORLD HEIGHT"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(28.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "2160", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 1
	);

	this->inputFields["BORDERS THICKNESS"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(35.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "32", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 2
	);

	this->inputFields["ANIMALS COUNT"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(42.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "4", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 3
	);

	this->inputFields["FRUITS COUNT"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(49.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "1024", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 4
	);

	this->inputFields["DEFAULT HP"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(56.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "100000000", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 5
	);

	this->inputFields["DEFAULT FRUIT ENERGY"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(63.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "100000000", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 6
	);

	this->inputFields["MUTATION RATE"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(70.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "100", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 7
	);

	this->inputFields["NAME"] = std::make_unique<gui::InputField>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(77.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		this->fonts["RETROICA"], "Default name", charSize,
		sf::Color(100, 100, 100), sf::Color(125, 125, 125), sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), sf::Color(100, 100, 100), sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), sf::Color(255, 255, 255), sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 0.5f,
		false, 8
	);
}

void EcosystemCreatorState::initTexts()
{
	const sf::VideoMode& resolution = this->stateData->gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(20.0f, resolution);

	this->addText(
		"WORLD WIDTH",
		"WORLD WIDTH",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(22.1f, resolution)
		)
	);

	this->addText(
		"WORLD HEIGHT",
		"WORLD HEIGHT",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(29.1f, resolution)
		)
	);

	this->addText(
		"BORDERS THICKNESS",
		"BORDERS THICKNESS",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(36.1f, resolution)
		)
	);

	this->addText(
		"ANIMALS COUNT",
		"ANIMALS COUNT",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(43.1f, resolution)
		)
	);

	this->addText(
		"FRUITS COUNT",
		"FRUITS COUNT",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(50.1f, resolution)
		)
	);

	this->addText(
		"DEFAULT HP",
		"DEFAULT HP",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(57.1f, resolution)
		)
	);

	this->addText(
		"DEFAULT FRUITS ENERGY",
		"DEFAULT FRUITS ENERGY",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(64.1f, resolution)
		)
	);

	this->addText(
		"MUTATION RATE",
		"MUTATION RATE",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(71.1f, resolution)
		)
	);

	this->addText(
		"ECOSYSTEM NAME",
		"ECOSYSTEM NAME",
		this->fonts["RETROICA"],
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.f, resolution),
			gui::p2pY(78.1f, resolution)
		)
	);
}

// other private methods:
void EcosystemCreatorState::addText(
	const std::string& str,
	const std::string& hash_key,
	const sf::Font& font,
	unsigned char_size,
	const sf::Color& color,
	const sf::Vector2f& position)
{
	this->texts[hash_key] = std::make_unique<sf::Text>(str, font, char_size);
	this->texts[hash_key]->setFillColor(color);
	this->texts[hash_key]->setPosition(position);
}

void EcosystemCreatorState::loadEcosystemTemplate(const std::string& ecosystem_name)
{
	std::string path = "resources/ecosystems templates/" + ecosystem_name + ".ini";

	std::ifstream file(path);

	if (!file.is_open())
		throw("ERROR::EcosystemCreatorState::useTemplate::COULD NOT OPEN: " + path + '\n');

	// read some variables:
	sf::Vector2f worldSize;
	float bordersThickness;
	unsigned animalsCount, fruitsCount;
	float defaultHp, defaultFruitEnergy, mutationRate;

	file >> worldSize.x >> worldSize.y;
	file >> bordersThickness;
	file >> animalsCount >> fruitsCount;
	file >> defaultHp >> defaultFruitEnergy >> mutationRate;

	std::string strWorldWidth = this->removeFloatTrailingZeros(std::to_string(worldSize.x));
	std::string strWorldHeight = this->removeFloatTrailingZeros(std::to_string(worldSize.y));
	std::string strBordersThickness = this->removeFloatTrailingZeros(std::to_string(bordersThickness));
	std::string strAnimalsCount = this->removeFloatTrailingZeros(std::to_string(animalsCount));
	std::string strFruitsCount = this->removeFloatTrailingZeros(std::to_string(fruitsCount));
	std::string strDefaultHp = this->removeFloatTrailingZeros(std::to_string(defaultHp));
	std::string strDefaultFruitEnergy = this->removeFloatTrailingZeros(std::to_string(defaultFruitEnergy));
	std::string strMutationRate = this->removeFloatTrailingZeros(std::to_string(mutationRate));

	this->inputFields["WORLD WIDTH"]->setString(strWorldWidth);

	this->inputFields["WORLD HEIGHT"]->setString(strWorldHeight);

	this->inputFields["BORDERS THICKNESS"]->setString(strBordersThickness);

	this->inputFields["ANIMALS COUNT"]->setString(strAnimalsCount);

	this->inputFields["FRUITS COUNT"]->setString(strFruitsCount);

	this->inputFields["DEFAULT HP"]->setString(strDefaultHp);

	this->inputFields["DEFAULT FRUIT ENERGY"]->setString(strDefaultFruitEnergy);

	this->inputFields["MUTATION RATE"]->setString(strMutationRate);

	this->inputFields["NAME"]->setString(ecosystem_name);

	file.close();
}

std::string EcosystemCreatorState::removeFloatTrailingZeros(const std::string& string)
{
	bool periodOccurs = false;

	for (const auto& it : string)
		if (it == '.')
		{
			periodOccurs = true;
			break;
		}

	if (!periodOccurs) 
		return string;

	unsigned index = string.size();

	while (index > 0 && string[index - 1] == '0') 
		index--;

	if (string[index - 1] == '.') 
		index--;

	std::string temp = string;

	return temp.substr(0, index);
}

void EcosystemCreatorState::createEcosystem()
{
	delete this->stateData->ecosystem;

	this->stateData->ecosystem = new Ecosystem(
		this->inputFields["NAME"]->getInput(),
		sf::Vector2f(
			std::stof(this->inputFields["WORLD WIDTH"]->getInput()),
			std::stof(this->inputFields["WORLD HEIGHT"]->getInput())
		),
		std::stof(this->inputFields["BORDERS THICKNESS"]->getInput()),
		sf::Color(32, 32, 32),
		sf::Color(48, 48, 48),
		std::stoi(this->inputFields["ANIMALS COUNT"]->getInput()),
		std::stoi(this->inputFields["FRUITS COUNT"]->getInput()),
		8.0f, // TODO: do sth with that hard-coded thing
		4.0f, // TODO: do sth with that hard-coded thing
		std::stof(this->inputFields["DEFAULT HP"]->getInput()),
		std::stof(this->inputFields["DEFAULT FRUIT ENERGY"]->getInput()),
		std::stof(this->inputFields["MUTATION RATE"]->getInput()),
		sf::Color::Red, // TODO: do sth with that hard-coded thing
		sf::Color::Green, // TODO: do sth with that hard-coded thing
		sf::Color(100, 0, 200), // TODO: do sth with that hard-coded thing
		1.0f, // TODO: do sth with that hard-coded thing
		true,
		GodTool::NONE,
		true,
		false
	);
}

void EcosystemCreatorState::updateInput()
{
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"])) && !this->keysBlockades["CLOSE"])
		this->endState();
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"])))
		this->endState();
}

void EcosystemCreatorState::updateGui(float dt)
{
	for (auto& button : this->buttons)
		button.second->update(this->mousePosWindow);

	for (auto& inputField : this->inputFields)
		inputField.second->update(dt, *this->stateData->events, this->mousePosWindow);
}

void EcosystemCreatorState::getUpdatesFromGui()
{
	if (this->buttons["SMALL"]->isClicked())
		this->loadEcosystemTemplate("small");

	else if (this->buttons["BIG"]->isClicked())
		this->loadEcosystemTemplate("big");

	else if (this->buttons["HUGE"]->isClicked())
		this->loadEcosystemTemplate("huge");

	else if (this->buttons["CREATE"]->isClicked())
		this->createEcosystem();

	else if (this->buttons["OK"]->isClicked())
		this->endState();
}

void EcosystemCreatorState::renderGui(sf::RenderTarget& target)
{
	for (const auto& button : this->buttons)
		button.second->render(target);

	for (const auto& inputField : this->inputFields)
		inputField.second->render(target);

	for (const auto& text : this->texts)
		target.draw(*text.second);
}
