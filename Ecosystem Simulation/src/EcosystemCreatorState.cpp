#include "EcosystemCreatorState.h"

EcosystemCreatorState::EcosystemCreatorState(StateData* stateData)
	: State(stateData)
	, m_background()
	, m_font()
	, m_texts()
	, m_buttons()
	, m_textBoxes()
{
	initKeybinds();
	initBackground();
	initFont();
	initGui();
}

void EcosystemCreatorState::update(float dt)
{
	updateMousePositions();
	updateInput();
	updateGui(dt);
	getUpdatesFromGui();
}

void EcosystemCreatorState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = m_stateData->m_window;
	}

	target->draw(m_background);

	renderGui(*target);
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
		{
			m_keybinds[key] = m_stateData->m_supportedKeys->at(key2);
		}
	}
	else
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::EcosystemCreatorState::initKeybinds()::"
			<< "could not open "
			<< path << '\n'
		);
	}

	ifs.close();
}

void EcosystemCreatorState::initBackground()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	m_background.setSize(
		sf::Vector2f
		(
			static_cast<float>(resolution.width),
			static_cast<float>(resolution.height)
		)
	);

	m_background.setFillColor(sf::Color(32, 32, 32));
}

void EcosystemCreatorState::initFont()
{
	const char* filePath = "resources/fonts/Retroica.ttf";

	if (!m_font.loadFromFile(filePath))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::EcosystemCreatorState::initFonts()::"
			<< "could not load "
			<< filePath << '\n'
		);
	}
}

void EcosystemCreatorState::initGui()
{
	initButtons();
	initTextBoxes();
	initTexts();
}

void EcosystemCreatorState::initButtons()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(20.0f, resolution);

	m_buttons["SMALL"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(30.0f, resolution),
			gui::p2pY(7.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, "SMALL", charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_buttons["BIG"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(7.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, "BIG", charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_buttons["HUGE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(58.0f, resolution),
			gui::p2pY(7.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, "HUGE", charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_buttons["CREATE"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(71.0f, resolution),
			gui::p2pY(90.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, "CREATE", charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);

	m_buttons["OK"] = std::make_unique<gui::Button>(
		sf::Vector2f(
			gui::p2pX(84.0f, resolution),
			gui::p2pY(90.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, "OK", charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64),
		sf::Color(100, 100, 100),
		sf::Color(48, 48, 48),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255),
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution)
	);
}

void EcosystemCreatorState::initTextBoxes()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(20.0f, resolution);

	m_textBoxes["WORLD WIDTH"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(21.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"3840", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64),
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false
	);

	m_textBoxes["WORLD HEIGHT"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(28.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"2160", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false,
		1
	);

	m_textBoxes["BORDERS THICKNESS"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(35.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"32", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false, 
		2
	);

	m_textBoxes["ANIMALS COUNT"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(42.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"4", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false, 
		3
	);

	m_textBoxes["FRUITS COUNT"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(49.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"1024", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false, 
		4
	);

	m_textBoxes["DEFAULT HP"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(56.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"100000000", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution),
		0.5f,
		false, 
		5
	);

	m_textBoxes["DEFAULT FRUIT ENERGY"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(63.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"100000000", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false, 
		6
	);

	m_textBoxes["MUTATION RATE"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(70.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font,
		"100", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false, 
		7
	);

	m_textBoxes["NAME"] = std::make_unique<gui::TextBox>(
		sf::Vector2f(
			gui::p2pX(44.0f, resolution),
			gui::p2pY(77.0f, resolution)
		),
		sf::Vector2f(
			gui::p2pX(12.0f, resolution),
			gui::p2pY(5.0f, resolution)
		),
		m_font, 
		"Default name", 
		charSize,
		sf::Color(100, 100, 100), 
		sf::Color(125, 125, 125), 
		sf::Color(75, 75, 75),
		sf::Color(64, 64, 64), 
		sf::Color(100, 100, 100), 
		sf::Color(32, 32, 32),
		sf::Color(225, 225, 225), 
		sf::Color(255, 255, 255), 
		sf::Color(150, 150, 150),
		gui::p2pY(0.5f, resolution), 
		gui::p2pY(100.0f * 1.0f / 1080.0f, resolution), 
		0.5f,
		false, 
		8
	);
}

void EcosystemCreatorState::initTexts()
{
	const sf::VideoMode& resolution = m_stateData->m_gfxSettings->resolution;

	unsigned charSize = gui::calcCharSize(20.0f, resolution);

	addText(
		"WORLD WIDTH",
		"WORLD WIDTH",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(22.1f, resolution)
		)
	);

	addText(
		"WORLD HEIGHT",
		"WORLD HEIGHT",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(29.1f, resolution)
		)
	);

	addText(
		"BORDERS THICKNESS",
		"BORDERS THICKNESS",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(36.1f, resolution)
		)
	);

	addText(
		"ANIMALS COUNT",
		"ANIMALS COUNT",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(43.1f, resolution)
		)
	);

	addText(
		"FRUITS COUNT",
		"FRUITS COUNT",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(50.1f, resolution)
		)
	);

	addText(
		"DEFAULT HP",
		"DEFAULT HP",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(57.1f, resolution)
		)
	);

	addText(
		"DEFAULT FRUITS ENERGY",
		"DEFAULT FRUITS ENERGY",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(64.1f, resolution)
		)
	);

	addText(
		"MUTATION RATE",
		"MUTATION RATE",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(71.1f, resolution)
		)
	);

	addText(
		"ECOSYSTEM NAME",
		"ECOSYSTEM NAME",
		m_font,
		charSize,
		sf::Color(225, 225, 225),
		sf::Vector2f(
			gui::p2pX(24.0f, resolution),
			gui::p2pY(78.1f, resolution)
		)
	);
}

// utils:

void EcosystemCreatorState::addText(
	const std::string& str,
	const std::string& hashKey,
	const sf::Font& font,
	unsigned charSize,
	const sf::Color& color,
	const sf::Vector2f& position)
{
	m_texts[hashKey] = std::make_unique<sf::Text>(str, font, charSize);
	m_texts[hashKey]->setFillColor(color);
	m_texts[hashKey]->setPosition(position);
}

void EcosystemCreatorState::loadEcosystemTemplate(
	const std::string& ecosystemName)
{
	std::string path = "resources/ecosystems templates/";
	path += ecosystemName.c_str();
	path += ".ini";

	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::EcosystemCreatorState::"
			<< "useTemplate(const std::string&)::"
			<< "could not open "
			<< path << '\n'
		);
	}

	// read some variables:
	sf::Vector2f worldSize;
	float bordersThickness;
	unsigned animalsCount, fruitsCount;
	unsigned defaultHp, defaultFruitEnergy;
	float mutationRate;

	file >> worldSize.x >> worldSize.y;
	file >> bordersThickness;
	file >> animalsCount >> fruitsCount;
	file >> defaultHp >> defaultFruitEnergy >> mutationRate;

	std::string strWorldWidth = std::to_string(worldSize.x);
	std::string strWorldHeight = std::to_string(worldSize.y);
	std::string strBordersThickness = std::to_string(bordersThickness);
	std::string strAnimalsCount = std::to_string(animalsCount);
	std::string strFruitsCount = std::to_string(fruitsCount);
	std::string strDefaultHp = std::to_string(defaultHp);
	std::string strDefaultFruitEnergy = std::to_string(defaultFruitEnergy);
	std::string strMutationRate = std::to_string(mutationRate);

	strWorldWidth = removeFloatTrailingZeros(strWorldWidth);
	strWorldHeight = removeFloatTrailingZeros(strWorldHeight);
	strBordersThickness = removeFloatTrailingZeros(strBordersThickness);
	strAnimalsCount = removeFloatTrailingZeros(strAnimalsCount);
	strFruitsCount = removeFloatTrailingZeros(strFruitsCount);
	strDefaultHp = removeFloatTrailingZeros(strDefaultHp);
	strDefaultFruitEnergy = removeFloatTrailingZeros(strDefaultFruitEnergy);
	strMutationRate = removeFloatTrailingZeros(strMutationRate);

	m_textBoxes["WORLD WIDTH"]->setString(strWorldWidth);
	m_textBoxes["WORLD HEIGHT"]->setString(strWorldHeight);
	m_textBoxes["BORDERS THICKNESS"]->setString(strBordersThickness);
	m_textBoxes["ANIMALS COUNT"]->setString(strAnimalsCount);
	m_textBoxes["FRUITS COUNT"]->setString(strFruitsCount);
	m_textBoxes["DEFAULT HP"]->setString(strDefaultHp);
	m_textBoxes["DEFAULT FRUIT ENERGY"]->setString(strDefaultFruitEnergy);
	m_textBoxes["MUTATION RATE"]->setString(strMutationRate);
	m_textBoxes["NAME"]->setString(ecosystemName);

	file.close();
}

std::string EcosystemCreatorState::removeFloatTrailingZeros(
	const std::string& string)
{
	bool periodOccurs = false;

	for (const auto& c : string)
	{
		if (c == '.')
		{
			periodOccurs = true;
			break;
		}
	}

	if (!periodOccurs) return string;

	unsigned index = string.size();

	while (index > 0U && string[index - 1U] == '0') index--;

	if (string[index - 1U] == '.') index--;

	return string.substr(0U, index);
}

void EcosystemCreatorState::createEcosystem()
{
	// TODO: do sth with that hard-coded thing
	*m_stateData->m_ecosystem = Ecosystem(
		m_textBoxes["NAME"]->getInput(),
		sf::Vector2f(
			std::stof(m_textBoxes["WORLD WIDTH"]->getInput()),
			std::stof(m_textBoxes["WORLD HEIGHT"]->getInput())
		),
		std::stof(m_textBoxes["BORDERS THICKNESS"]->getInput()),
		sf::Color(
			32, 
			32, 
			32
		),
		sf::Color(
			48, 
			48, 
			48
		),
		std::stoi(m_textBoxes["ANIMALS COUNT"]->getInput()),
		std::stoi(m_textBoxes["FRUITS COUNT"]->getInput()),
		8.0f,
		4.0f,
		static_cast<unsigned>(std::stoi(m_textBoxes["DEFAULT HP"]->getInput())),
		static_cast<unsigned>(std::stoi(m_textBoxes["DEFAULT FRUIT ENERGY"]->getInput())),
		std::stof(m_textBoxes["MUTATION RATE"]->getInput()),
		sf::Color::Red,
		sf::Color::Green,
		sf::Color(
			100, 
			0, 
			200
		),
		1.0f,
		true,
		GodTool::NONE,
		true,
		true
	);
}

void EcosystemCreatorState::updateInput()
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["CLOSE"])) 
	//	&& !keysBlockades["CLOSE"])
	//{
	//	endState();
	//}	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["CLOSE"])))
	{
		endState();
	}
}

void EcosystemCreatorState::updateGui(float dt)
{
	for (auto& button : m_buttons)
	{
		button.second->update(static_cast<sf::Vector2f>(m_mousePosWindow));
	}

	for (auto& textBox : m_textBoxes)
	{
		textBox.second->update(dt, *m_stateData->m_events, m_mousePosWindow);
	}
}

void EcosystemCreatorState::getUpdatesFromGui()
{
	if (m_buttons["SMALL"]->isClicked())
	{
		loadEcosystemTemplate("small");
	}
	else if (m_buttons["BIG"]->isClicked())
	{
		loadEcosystemTemplate("big");
	}
	else if (m_buttons["HUGE"]->isClicked())
	{
		loadEcosystemTemplate("huge");
	}
	else if (m_buttons["CREATE"]->isClicked())
	{
		createEcosystem();
	}
	else if (m_buttons["OK"]->isClicked())
	{
		endState();
	}
}

void EcosystemCreatorState::renderGui(sf::RenderTarget& target)
{
	for (const auto& button : m_buttons)
	{
		button.second->render(target);
	}

	for (const auto& textBox : m_textBoxes)
	{
		textBox.second->render(target);
	}

	for (const auto& text : m_texts)
	{
		target.draw(*text.second);
	}
}
