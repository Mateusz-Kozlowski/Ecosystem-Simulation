#include "Ecosystem.h"

Ecosystem::Ecosystem()
	: m_name("")
	, m_background()
	, m_animals()
	, m_fruits()
	, m_animalsRadius(0.0f)
	, m_fruitsRadius(0.0f)
	, m_defaultAnimalsHp(0U)
	, m_defaultFruitEnergy(0U)
	, m_mutationsPerMutation(0U)
	, m_animalsColor(sf::Color::Magenta)
	, m_fruitsColor(sf::Color::Magenta)
	, m_trackedAnimalColor(sf::Color::Magenta)
	, m_trackedAnimal(nullptr)
	, m_animalWithModifiedBrain(nullptr)
	, m_simulationSpeedFactor(1.0f)
	, m_simulationIsPaused(true)
	, m_godTool(GodTool::NONE)
	, m_hpBarsVisibility()
	, m_brainsVisibility()
	, m_totalTimeElapsed(0.0f)
{

}

Ecosystem::Ecosystem(
	const std::string& name,
	const sf::Vector2f& worldSize,
	float bordersThickness,
	const sf::Color& backgroundColor,
	const sf::Color& bordersColor,
	unsigned animalsCount,
	unsigned fruitsCount,
	float animalsRadius,
	float fruitsRadius,
	unsigned defaultAnimalsHp,
	unsigned defaultFruitsEnergy,
	unsigned mutationsPerMutation,
	const sf::Color& animalsColor,
	const sf::Color& fruitsColor,
	const sf::Color& trackedAnimalColor,
	float simulationSpeedFactor,
	bool simulationIsPaused,
	GodTool godTool,
	bool renderHpBarsByDefault,
	bool renderBrainsByDefault)
	: m_name(name)
	, m_background()
	, m_animals()
	, m_fruits()
	, m_animalsRadius(animalsRadius)
	, m_defaultAnimalsHp(defaultAnimalsHp)
	, m_defaultFruitEnergy(defaultFruitsEnergy)
	, m_fruitsRadius(fruitsRadius)
	, m_mutationsPerMutation(mutationsPerMutation)
	, m_animalsColor(animalsColor)
	, m_fruitsColor(fruitsColor)
	, m_trackedAnimalColor(trackedAnimalColor)
	, m_trackedAnimal(nullptr)
	, m_animalWithModifiedBrain(nullptr)
	, m_simulationSpeedFactor(simulationSpeedFactor)
	, m_simulationIsPaused(simulationIsPaused)
	, m_godTool(godTool)
	, m_hpBarsVisibility()
	, m_brainsVisibility()
	, m_totalTimeElapsed(0.0f)
{
	std::cout << mutationsPerMutation << '\n';

	initBackgroundAndBorders(
		worldSize, 
		bordersThickness, 
		backgroundColor, 
		bordersColor
	);
	createNewAnimals(
		animalsCount,
		defaultAnimalsHp,
		animalsRadius,
		animalsColor,
		renderHpBarsByDefault,
		renderBrainsByDefault
	);
	createNewFruits(
		fruitsCount, 
		defaultFruitsEnergy, 
		fruitsRadius, 
		fruitsColor
	);
}

Ecosystem::Ecosystem(const char* folderPath)
	: m_name("NO NAME HAS BEEN SET YET")
	, m_background()
	, m_animals()
	, m_fruits()
	, m_animalsRadius(0.0f)
	, m_fruitsRadius(0.0f)
	, m_defaultAnimalsHp(0U)
	, m_defaultFruitEnergy(0U)
	, m_mutationsPerMutation(0U)
	, m_animalsColor(sf::Color::Magenta)
	, m_fruitsColor(sf::Color::Magenta)
	, m_trackedAnimalColor(sf::Color::Magenta)
	, m_trackedAnimal(nullptr)
	, m_animalWithModifiedBrain(nullptr)
	, m_simulationSpeedFactor(1.0f)
	, m_simulationIsPaused(true)
	, m_godTool(GodTool::NONE)
	, m_hpBarsVisibility()
	, m_brainsVisibility()
	, m_totalTimeElapsed(0.0f)
{
	loadFromFolder(folderPath);
}

void Ecosystem::saveToFolder(const std::string& folderPath) const
{
	if (folderPath != "")
	{
		std::filesystem::create_directories(folderPath);
	}
	
	// TODO: add static private (or sth even better) 
	// TODO: methods that returns those paths:
	saveAnimals(folderPath + "/animals");
	saveFruits(folderPath + "/fruits");
	saveEcosystem(folderPath + "/ecosystem.ini");
	saveHpBarsVisibility(folderPath + "/hp bars visibility.ini");
	saveBrainsPreviewsVisibility(
		folderPath + "/brains previews visibility.ini"
	);
}

void Ecosystem::loadFromFolder(const std::string& folderPath)
{
	if (!std::filesystem::is_directory(folderPath)) return;

	// TODO: add static private (or sth even better) 
	// TODO: methods that returns those paths:
	loadAnimals(folderPath + "/animals");
	loadFruits(folderPath + "/fruits");
	loadEcosystem(folderPath + "/ecosystem.ini");
	loadHpBarsVisibility(folderPath + "/hp bars visibility.ini");
	loadBrainsPreviewsVisibility(
		folderPath + "/brains previews visibility.ini"
	);
}

void Ecosystem::useGodTools(
	const std::vector<sf::Event>& events,
	const sf::Vector2f& mousePos)
{
	bool eventHasOccured = EventsAccessor::hasEventOccured(
		sf::Event::MouseButtonPressed, 
		events
	);

	if (!eventHasOccured) return;
	
	// now use God tool:
	switch (m_godTool)
	{
	case GodTool::NONE:
		return;

	case GodTool::MUTATE:
		mutatingTool(mousePos, events);
		return;

	case GodTool::TRACK:
		trackingTool(mousePos);
		return;

	case GodTool::KILL:
		killingTool(mousePos);
		return;

	case GodTool::REPLACE:
		replacingTool(mousePos);
		return;

	case GodTool::BRAIN:
		brainTool(mousePos);
		return;

	case GodTool::STOP:
		stoppingTool(mousePos);
		return;

	case GodTool::INFO:
		infoTool(mousePos);
		return;

	default:
		std::cerr
			<< "Error::Ecosystem::useGodTool"
			<< "(const std::vector<sf::Event>&, const sf::Vector2f&)::"
			<< "there is no God tool like "
			<< m_godTool << '\n';
		assert(false);
	}
}

void Ecosystem::update(
	float dt,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	if (m_simulationIsPaused)
	{
		updateOnlyImgBtnsOfBrainsPreviews(mousePos, events);
	}
	else
	{
		m_totalTimeElapsed += dt;
		updateWorld(dt, mousePos, events);
	}
	
	updateModifyingBrainsPreviews();
}

void Ecosystem::render(sf::RenderTarget& target) const
{
	target.draw(m_background);

	for (const auto& animal : m_animals)
	{
		animal->renderBody(target);
	}

	for (const auto& animal : m_animals)
	{
		if (m_hpBarsVisibility.at(animal.get()))
		{
			animal->renderHpBar(target);
		}
	}

	for (const auto& fruit : m_fruits)
	{
		fruit->render(target);
	}

	for (const auto& animal : m_animals)
	{
		if (m_brainsVisibility.at(animal.get()))
		{
			animal->renderBrainPreview(target);
		}
	}
}

// accessors:

const std::string& Ecosystem::getName() const
{
	return m_name;
}

float Ecosystem::calcFruitsDensity() const
{
	return m_fruits.size() / calcArenaArea();
}

float Ecosystem::calcArenaArea() const
{
	return getArenaSize().x * getArenaSize().y;
}

const sf::Vector2f& Ecosystem::getArenaSize() const
{
	return m_background.getSize();
}

sf::Vector2f Ecosystem::getWorldSize() const
{
	return {
		m_background.getSize().x + 2 * m_background.getOutlineThickness(),
		m_background.getSize().y + 2 * m_background.getOutlineThickness()
	};
}

float Ecosystem::getBordersThickness() const
{
	return m_background.getOutlineThickness();
}

unsigned Ecosystem::getAnimalsCount() const
{
	return m_animals.size();
}

const std::vector<std::shared_ptr<Animal>>& Ecosystem::getAnimals() const
{
	return m_animals;
}

unsigned Ecosystem::getFruitsCount() const
{
	return m_fruits.size();
}

unsigned Ecosystem::getMutationsPerMutation() const
{
	return m_mutationsPerMutation;
}

const sf::Color& Ecosystem::getBackgroundColor() const
{
	return m_background.getFillColor();
}

const sf::Color& Ecosystem::getBordersColor() const
{
	return m_background.getOutlineColor();
}

const sf::Color& Ecosystem::getAnimalsColor() const
{
	return m_animalsColor;
}

const sf::Color& Ecosystem::getFruitsColor() const
{
	return m_fruitsColor;
}

const sf::Color& Ecosystem::getTrackedAnimalColor() const
{
	return m_trackedAnimalColor;
}

const Animal* Ecosystem::getTrackedAnimal() const
{
	return m_trackedAnimal;
}

const Animal* Ecosystem::getAnimalWithModifiedBrain() const
{
	return m_animalWithModifiedBrain;
}

float Ecosystem::getSimulationSpeedFactor() const
{
	return m_simulationSpeedFactor;
}

bool Ecosystem::isSimulationPaused() const
{
	return m_simulationIsPaused;
}

GodTool Ecosystem::getCurrentGodTool() const
{
	return m_godTool;
}

const std::unordered_map<Animal*, bool>& Ecosystem::getBrainsVisibility() const
{
	return m_brainsVisibility;
}

float Ecosystem::getTotalTimeElapsed() const
{
	return m_totalTimeElapsed;
}

unsigned Ecosystem::getTotalAnimalsHpEnergy() const
{
	unsigned totalHpEnergy = 0.0;

	for (const auto& animal : m_animals)
	{
		totalHpEnergy += animal->getHp();
	}

	return totalHpEnergy;
}

unsigned Ecosystem::getTotalAnimalsKineticEnergy() const
{
	unsigned totalKineticEnergy = 0U;

	for (const auto& animal : m_animals)
	{
		totalKineticEnergy += animal->getKineticEnergy();
	}

	return totalKineticEnergy;
}

unsigned Ecosystem::getTotalFruitsEnergy() const
{
	unsigned totalFruitsEnergy = 0U;

	for (const auto& fruit : m_fruits)
	{
		totalFruitsEnergy += fruit->getEnergy();
	}

	return totalFruitsEnergy;
}

unsigned Ecosystem::getTotalEnergy(bool print) const
{
	unsigned hp = getTotalAnimalsHpEnergy();
	unsigned fE = getTotalFruitsEnergy();
	unsigned kE = getTotalAnimalsKineticEnergy();

	if (print)
	{
		std::cout << "hp: " << hp << '\n';
		std::cout << "fE: " << fE << '\n';
		std::cout << "kE: " << kE << '\n';
	}

	return hp + fE + kE;

	/*
	return getTotalAnimalsHpEnergy()
		 + getTotalAnimalsKineticEnergy()
		 + getTotalFruitsEnergy();*/
}

void Ecosystem::printAllAnimalsPositions() const
{
	for (const auto& animal : m_animals)
	{
		std::cout
			<< animal->getPosition().x
			<< ' '
			<< animal->getPosition().y
			<< '\n';
	}
}

void Ecosystem::printInfo() const
{
	printInfoAboutEcosystem();
}

// mutators:

void Ecosystem::setName(const std::string& name)
{
	m_name = name;
}

void Ecosystem::setMutationsCount(float mutationRate)
{
	m_mutationsPerMutation = mutationRate;
}

void Ecosystem::setBackgroundColor(const sf::Color& backgroundColor)
{
	m_background.setFillColor(backgroundColor);
}

void Ecosystem::setBordersColor(const sf::Color& bordersColor)
{
	m_background.setOutlineColor(bordersColor);
}

void Ecosystem::setAnimalsColor(const sf::Color& animalsColor)
{
	m_animalsColor = animalsColor;

	for (auto& animal : m_animals)
	{
		if (animal.get() != m_trackedAnimal)
		{
			animal->setColor(animalsColor);
		}
	}
}

void Ecosystem::setFruitsColor(const sf::Color& fruitsColor)
{
	m_fruitsColor = fruitsColor;

	for (auto& fruit : m_fruits)
	{
		fruit->setColor(fruitsColor);
	}
}

void Ecosystem::setTrackedAnimalColor(const sf::Color& trackedAnimalColor)
{
	m_trackedAnimalColor = trackedAnimalColor;

	if (m_trackedAnimal)
	{
		m_trackedAnimal->setColor(trackedAnimalColor);
	}
}

void Ecosystem::setSimulationSpeedFactor(float simulationSpeedFactor)
{
	m_simulationSpeedFactor = simulationSpeedFactor;
}

void Ecosystem::pauseSimulation()
{
	m_simulationIsPaused = true;
}

void Ecosystem::unpauseSimulation()
{
	m_simulationIsPaused = false;
}

void Ecosystem::setGodTool(GodTool godTool)
{
	m_godTool = godTool;
}

void Ecosystem::hideAllBrainsPreviews()
{
	for (auto& it : m_brainsVisibility)
	{
		it.second = false;
	}
}

void Ecosystem::showAllBrainsPreviews()
{
	for (auto& it : m_brainsVisibility)
	{
		it.second = true;
	}
}

void Ecosystem::stopModifyingBrainPreview()
{
	m_animalWithModifiedBrain = nullptr;
}

// private methods:

// initialization:

void Ecosystem::initBackgroundAndBorders(
	const sf::Vector2f& worldSize,
	float bordersThickness,
	const sf::Color& backgroundColor,
	const sf::Color& bordersColor)
{
	m_background.setSize(
		sf::Vector2f(
			worldSize.x - 2.0f * bordersThickness,
			worldSize.y - 2.0f * bordersThickness
		)
	);
	m_background.setPosition(bordersThickness, bordersThickness);
	m_background.setFillColor(backgroundColor);

	m_background.setOutlineThickness(bordersThickness);
	m_background.setOutlineColor(bordersColor);
}

void Ecosystem::createNewAnimals(
	unsigned animalsCount,
	unsigned defaultAnimalsHp,
	float animalsRadius,
	const sf::Color& animalsColor,
	bool renderHpBarsByDefault,
	bool renderBrainsByDefault)
{
	m_animals.clear();

	for (int i = 0; i < animalsCount; i++)
	{
		createNewAnimal(
			defaultAnimalsHp,
			animalsRadius,
			animalsColor,
			renderHpBarsByDefault,
			renderBrainsByDefault
		);
	}
}

void Ecosystem::createNewAnimal(
	unsigned defaultAnimalHp,
	float animalRadius,
	const sf::Color& animalColor,
	bool renderHpBarByDefault,
	bool renderBrainByDefault)
{
	m_animals.push_back(
		std::make_unique<Animal>(
			sf::Vector2f(
				getWorldSize().x / 2.0f,
				getWorldSize().y / 2.0f
			),
			animalRadius,
			animalColor,
			sf::Color(100, 100, 100),
			sf::Color::Red,
			defaultAnimalHp
		)
	);
	m_animals.back()->setRandomPosition(getWorldSize(), getBordersThickness());

	m_hpBarsVisibility[m_animals.back().get()] = renderHpBarByDefault;
	m_brainsVisibility[m_animals.back().get()] = renderBrainByDefault;
}

void Ecosystem::createNewFruits(
	unsigned fruitsCount,
	unsigned defaultFruitsEnergy,
	float fruitsRadius,
	const sf::Color& fruitsColor)
{
	m_fruits.clear();

	for (int i = 0; i < fruitsCount; i++)
	{
		createNewFruit(defaultFruitsEnergy, fruitsRadius, fruitsColor);
	}
}

void Ecosystem::createNewFruit(
	unsigned energy,
	float radius, 
	const sf::Color& fruitColor)
{
	m_fruits.push_back(
		std::make_unique<Fruit>(
			energy,
			sf::Vector2f(
				0.0f,
				0.0f
			),
			radius,
			fruitColor
		)
	);
	m_fruits.back()->setRandomPosition(
		getWorldSize(), 
		getBordersThickness(),
		0.1f // unhardcode margins
	);
}

// utils:

void Ecosystem::saveAnimals(const std::string& folderPath) const
{
	for (int i = 0; i < m_animals.size(); i++)
	{
		m_animals[i]->saveToFolder(
			std::string(
				folderPath
				+ "/animal"
				+ std::to_string(i)
			).c_str()
		);
	}
}

void Ecosystem::saveFruits(const std::string& folderPath) const
{
	std::filesystem::create_directories(folderPath);

	for (int i = 0; i < m_fruits.size(); i++)
	{
		m_fruits[i]->saveToFile(
			std::string(
				folderPath
				+ "/fruit"
				+ std::to_string(i) 
				+ ".ini"
			).c_str()
		);
	}
}

void Ecosystem::saveEcosystem(const std::string& filePath) const
{
	std::ofstream ofs(filePath);

	if (!ofs.is_open())
	{
		std::cerr
			<< "Error::saveToFolder(const std::string&)::"
			<< "cannot open: "
			<< filePath
			<< '\n';
		assert(false);
		return;
	}

	ofs << m_name << '\n';
	ofs << getWorldSize().x << ' ' << getWorldSize().y << '\n';
	ofs << m_background.getOutlineThickness() << '\n';

	ofs << static_cast<int>(m_background.getFillColor().r) << ' ';
	ofs << static_cast<int>(m_background.getFillColor().g) << ' ';
	ofs << static_cast<int>(m_background.getFillColor().b) << ' ';
	ofs << static_cast<int>(m_background.getFillColor().a) << '\n';

	ofs << static_cast<int>(m_background.getOutlineColor().r) << ' ';
	ofs << static_cast<int>(m_background.getOutlineColor().g) << ' ';
	ofs << static_cast<int>(m_background.getOutlineColor().b) << ' ';
	ofs << static_cast<int>(m_background.getOutlineColor().a) << '\n';

	ofs << m_defaultAnimalsHp << '\n';
	ofs << m_defaultFruitEnergy << '\n';
	ofs << m_mutationsPerMutation << '\n';

	ofs << static_cast<int>(m_animalsColor.r) << ' ';
	ofs << static_cast<int>(m_animalsColor.g) << ' ';
	ofs << static_cast<int>(m_animalsColor.b) << ' ';
	ofs << static_cast<int>(m_animalsColor.a) << '\n';

	ofs << static_cast<int>(m_fruitsColor.r) << ' ';
	ofs << static_cast<int>(m_fruitsColor.g) << ' ';
	ofs << static_cast<int>(m_fruitsColor.b) << ' ';
	ofs << static_cast<int>(m_fruitsColor.a) << '\n';

	ofs << static_cast<int>(m_trackedAnimalColor.r) << ' ';
	ofs << static_cast<int>(m_trackedAnimalColor.g) << ' ';
	ofs << static_cast<int>(m_trackedAnimalColor.b) << ' ';
	ofs << static_cast<int>(m_trackedAnimalColor.a) << '\n';

	ofs << getTrackedAnimalIndex() << '\n';

	ofs << m_simulationSpeedFactor << '\n';
	ofs << m_simulationIsPaused << '\n';
	ofs << m_godTool << '\n';
	ofs << m_totalTimeElapsed;

	ofs.close();
}

int Ecosystem::getTrackedAnimalIndex() const
{
	if (!m_trackedAnimal)
	{
		return -1;
	}

	for (int i = 0; i < m_animals.size(); i++)
	{
		if (m_animals[i].get() == m_trackedAnimal)
		{
			return i;
		}
	}

	std::cerr 
		<< "Error::Ecosystem::getTrackedAnimalIndex() const::"
		<< "trackedAnimal ptr points to a nonexistent animal\n";
	assert(false);

	return -1;
}

void Ecosystem::saveHpBarsVisibility(const std::string& filePath) const
{
	std::ofstream ofs(filePath);

	if (!ofs.is_open())
	{
		std::cerr
			<< "Error::Ecosystem::saveHpBarsVisibility"
			<< "(const std::string&) const::"
			<< "cannot open: "
			<< filePath
			<< '\n';
		assert(false);
		return;
	}

	for (const auto& it : m_hpBarsVisibility)
	{
		ofs << it.second << '\n';
	}

	ofs.close();
}

void Ecosystem::saveBrainsPreviewsVisibility(const std::string& filePath) const
{
	std::ofstream ofs(filePath);

	if (!ofs.is_open())
	{
		std::cerr 
			<< "Error::Ecosystem::"
			<< "saveBrainsPreviewsVisibility(const std::string&) const::"
			<< "cannot open: " 
			<< filePath 
			<< '\n';
		assert(false);
		return;
	}

	for (const auto& it : m_brainsVisibility)
	{
		ofs << it.second << '\n';
	}

	ofs.close();
}

void Ecosystem::loadAnimals(const std::string& folderPath)
{
	m_animals.clear();

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		loadAnimal(entry.path().string());
	}
}

void Ecosystem::loadAnimal(const std::string& folderPath)
{
	m_animals.push_back(std::make_unique<Animal>(folderPath.c_str()));
}

void Ecosystem::loadFruits(const std::string& folderPath)
{
	m_fruits.clear();

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		loadFruit(entry.path().string());
	}
}

void Ecosystem::loadFruit(const std::string& filePath)
{
	m_fruits.push_back(std::make_unique<Fruit>(filePath.c_str()));
}

void Ecosystem::loadEcosystem(const std::string& filePath)
{
	std::ifstream ifs(filePath);

	if (!ifs.is_open())
	{
		std::cerr
			<< "Error::Ecosystem::loadEcosystem(const std::string&)::"
			<< "cannot open: "
			<< filePath
			<< '\n';
		assert(false);
		return;
	}

	sf::Vector2f worldSize;
	float bordersThickness;
	unsigned bgColorR, bgColorG, bgColorB, bgColorA;
	unsigned bordersColorR, bordersColorG, bordersColorB, bordersColorA;
	int trackedAnimalIndex;
	unsigned animalsColorR, animalsColorG, animalsColorB, animalsColorA;
	unsigned fruitsColorR, fruitsColorG, fruitsColorB, fruitsColorA;
	unsigned trackedAnimalColorR,
			 trackedAnimalColorG,
			 trackedAnimalColorB,
			 trackedAnimalColorA;

	std::getline(ifs, m_name);
	
	ifs >> worldSize.x >> worldSize.y;
	ifs >> bordersThickness;

	ifs >> bgColorR >> bgColorG >> bgColorB >> bgColorA;
	ifs >> bordersColorR >> bordersColorG >> bordersColorB >> bordersColorA;

	ifs >> m_defaultAnimalsHp;
	ifs >> m_defaultFruitEnergy;
	ifs >> m_mutationsPerMutation;

	ifs >> animalsColorR >> animalsColorG >> animalsColorB >> animalsColorA;
	ifs >> fruitsColorR >> fruitsColorG >> fruitsColorB >> fruitsColorA;
	ifs >> trackedAnimalColorR 
		>> trackedAnimalColorG 
		>> trackedAnimalColorB 
		>> trackedAnimalColorA;

	ifs >> trackedAnimalIndex;

	ifs >> m_simulationSpeedFactor;
	ifs >> m_simulationIsPaused;
	ifs >> m_godTool;
	ifs >> m_totalTimeElapsed;

	ifs.close();

	initBackgroundAndBorders(
		worldSize,
		bordersThickness,
		sf::Color(
			bgColorR,
			bgColorG,
			bgColorB,
			bgColorA
		),
		sf::Color(
			bordersColorR,
			bordersColorG,
			bordersColorB,
			bordersColorA
		)
	);

	m_animalsColor = sf::Color(
		animalsColorR, 
		animalsColorG, 
		animalsColorB, 
		animalsColorA
	);

	m_fruitsColor = sf::Color(
		fruitsColorR, 
		fruitsColorG, 
		fruitsColorB, 
		fruitsColorA
	);

	m_trackedAnimalColor = sf::Color(
		trackedAnimalColorR, 
		trackedAnimalColorG, 
		trackedAnimalColorB, 
		trackedAnimalColorA
	);

	m_trackedAnimal = nullptr;

	if (trackedAnimalIndex != -1)
	{
		makeTracked(*m_animals[trackedAnimalIndex]);
	}
}

void Ecosystem::makeTracked(Animal& animal)
{
	if (m_trackedAnimal)
	{
		m_trackedAnimal->setColor(m_animalsColor);
	}
	
	m_trackedAnimal = &animal;

	animal.setColor(m_trackedAnimalColor);
}

void Ecosystem::loadHpBarsVisibility(const std::string& filePath)
{
	m_hpBarsVisibility.clear();

	std::ifstream ifs(filePath);

	if (!ifs.is_open())
	{
		std::cerr
			<< "Error::Ecosystem::loadHpBarsVisibility(const std::string&)::"
			<< "cannot open: "
			<< filePath
			<< '\n';
		assert(false);
		return;
	}

	bool hpBarVisibility;

	for (int i = 0; i < m_animals.size(); i++)
	{
		ifs >> hpBarVisibility;
		m_hpBarsVisibility[m_animals[i].get()] = hpBarVisibility;
	}

	ifs.close();
}

void Ecosystem::loadBrainsPreviewsVisibility(const std::string& filePath)
{
	m_brainsVisibility.clear();

	std::ifstream ifs(filePath);

	if (!ifs.is_open())
	{
		std::cerr 
			<< "ERROR::Ecosystem::loadBrainsPreviewsVisibility"
			<< "(const std::string&)::"
			<< "cannot open: " 
			<< filePath 
			<< '\n';
		assert(false);
		return;
	}

	bool brainPreviewVisibility;

	for (int i = 0; i < m_animals.size(); i++)
	{
		ifs >> brainPreviewVisibility;
		m_brainsVisibility[m_animals[i].get()] = brainPreviewVisibility;
	}

	ifs.close();
}

// God tools:

// mutating tool:

void Ecosystem::mutatingTool(
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	for (auto& animal : m_animals)
	{
		if (animal->isCoveredByMouse(mousePos))
		{
			animal->randomMutate(m_mutationsPerMutation, mousePos, events);
			return;
		}
	}
}

// tracking tool:

void Ecosystem::trackingTool(const sf::Vector2f& mousePos)
{
	for (auto& animal : m_animals)
	{
		if (animal->isCoveredByMouse(mousePos))
		{
			if (animal.get() == m_trackedAnimal)
			{
				stopTracking();
			}
			else
			{
				makeTracked(*animal);
			}

			return;
		}
	}
}

void Ecosystem::stopTracking()
{
	if (!m_trackedAnimal)
	{
		return;
	}

	m_trackedAnimal->setColor(m_animalsColor);
	m_trackedAnimal = nullptr;
}

// killing tool:

void Ecosystem::killingTool(const sf::Vector2f& mousePos)
{
	for (auto& animal : m_animals)
	{
		if (animal->isCoveredByMouse(mousePos))
		{
			convertAnimalToFruit(animal, false);
			return;
		}
	}
}

void Ecosystem::convertAnimalToFruit(
	std::shared_ptr<Animal>& animal, 
	bool randomFruitPosition)
{
	m_fruits.push_back(
		std::make_unique<Fruit>(
			animal->getTotalEnergy(),
			animal->getPosition(),
			m_fruitsRadius,
			m_fruitsColor
		)
	);

	if (randomFruitPosition)
	{
		m_fruits.back()->setRandomPosition(
			getWorldSize(), 
			getBordersThickness(),
			0.1f // unhardcode margins
		);
	}

	removeAnimal(animal);
}

// replacing tool:

void Ecosystem::replacingTool(const sf::Vector2f& mousePos)
{
	std::cout << "REPLACING TOOL IS NOT DEFINED YET!\n";
}

// brain tool:

void Ecosystem::brainTool(const sf::Vector2f& mousePos)
{
	for (auto& animal : m_animals)
	{
		if (animal->isCoveredByMouse(mousePos))
		{
			// TODO: it should be also updated if not all brains are updated every frame
			m_brainsVisibility[animal.get()]
				= !m_brainsVisibility[animal.get()];
			return;
		}
	}
}

// stopping tool:

void Ecosystem::stoppingTool(const sf::Vector2f& mousePos)
{
	for (auto& animal : m_animals)
	{
		if (animal->isCoveredByMouse(mousePos))
		{
			convertKineticEnergyToFruit(*animal, true);
			return;
		}
	}
}

void Ecosystem::convertKineticEnergyToFruit(
	Animal& animal, 
	bool randomFruitPosition)
{
	if (animal.getKineticEnergy() == 0U) return;

	m_fruits.push_back(
		std::make_unique<Fruit>(
			animal.getKineticEnergy(),
			animal.getPosition(),
			m_fruitsRadius,
			m_fruitsColor
		)
	);

	if (randomFruitPosition)
	{
		m_fruits.back()->setRandomPosition(
			getWorldSize(), 
			getBordersThickness(),
			0.1f // unhardcode margins
		);
	}

	animal.setVelocity(sf::Vector2i(0, 0));
}

// info tool:

void Ecosystem::infoTool(const sf::Vector2f& mousePos) const
{
	bool noInfoPrinted = true;

	for (const auto& animal : m_animals)
	{
		if (animal->isCoveredByMouse(mousePos))
		{
			noInfoPrinted = false;
			std::cout << '\n' << animal->toStr() << '\n';
		}
	}		

	for (const auto& fruit : m_fruits)
	{
		if (fruit->isCoveredByMouse(mousePos))
		{
			noInfoPrinted = false;
			std::cout << '\n' << fruit->toStr() << '\n';
		}
	}

	if (noInfoPrinted)
	{
		std::cout << '\n';
		printInfoAboutEcosystem();
	}
}

void Ecosystem::printInfoAboutEcosystem() const
{
	std::cout << "Info about the ecosystem:\n";
	std::cout << "name: " << m_name << '\n';
	std::cout << "arena size: "
		<< getWorldSize().x - 2U * getBordersThickness() << ' '
		<< getWorldSize().y - 2U * getBordersThickness() << '\n';
	std::cout << "borders thickness: " << getBordersThickness() << '\n';
	std::cout 
		<< "world size: " 
		<< getWorldSize().x << ' ' << getWorldSize().y << '\n';
	std::cout << "animals count: " << m_animals.size() << '\n';
	std::cout << "fruits count: " << m_fruits.size() << '\n';
	std::cout << "mutation rate percentage: " << m_mutationsPerMutation << '\n';
	std::cout 
		<< "simulation speed factor: " 
		<< m_simulationSpeedFactor << '\n';
	std::cout << "simulation is paused: " << m_simulationIsPaused << '\n';
	std::cout << "god tool: " << getGodToolStr(m_godTool) << '\n';
	std::cout << "total time elapsed [sec]: " << m_totalTimeElapsed << '\n';
	std::cout 
		<< "total animals hp energy: " 
		<< getTotalAnimalsHpEnergy() << '\n';
	std::cout 
		<< "total animals kinetic energy: "
		<< getTotalAnimalsKineticEnergy() << '\n';
	std::cout << "total fruits energy: " << getTotalFruitsEnergy() << '\n';
	std::cout << "total energy: " << getTotalEnergy() << '\n';
}

void Ecosystem::updateWorld(
	float dt,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	unsigned totalEnergy = getTotalEnergy();

	std::cout << "|||||-----NEW FRAME-----|||||:\n";

	updateAnimals(dt, mousePos, events);
	transferEnergyFromAnimalsToFruits();
	avoidTunneling();
	//kickInAssAnimalsStuckedNextToBorders();
	removeDeadAnimals();
	feedAnimals(dt, mousePos, events);
	removeEatenFruits();
	setHpBarsRanges();
	correctPopulationSize(dt);	
	correctBrainPreviewsPositions();
	correctFruitsCount();

	if (totalEnergy != getTotalEnergy())
	{
		std::cout << "BEFORE:\n";
		std::cout << "totalEnergy=" << totalEnergy << '\n';
		std::cout << "AFTER:\n";
		std::cout << "totalEnergy=" << getTotalEnergy() << '\n';
		sf::sleep(sf::seconds(4.0f));
		exit(-13);
	}
}

void Ecosystem::updateAnimals(
	float dt, 
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	for (const auto& animal : m_animals)
	{
		animal->update(
			dt, 
			m_simulationSpeedFactor, 
			getInputsForBrain(*animal),
			animal.get() == m_trackedAnimal,
			mousePos,
			events
		);
	}
}

std::vector<Blueberry::Scalar> Ecosystem::getInputsForBrain(
	const Animal& animal) const
{
	// TODO: add set BRAIN inputs methods in Animal class
	std::vector<Blueberry::Scalar> inputsForBrain;

	inputsForBrain.reserve(5);

	inputsForBrain.push_back(static_cast<Blueberry::Scalar>(
		animal.getVelocityVector().x)
	);
	inputsForBrain.push_back(static_cast<Blueberry::Scalar>(
		animal.getVelocityVector().y)
	);

	if (animal.getHp() <= 0)
	{
		std::cerr << "Assertion will fail, hp= " << animal.getHp() << '\n';
		exit(-13);
	}
	
	inputsForBrain.push_back(log2(animal.getHp()));

	const Fruit* theNearestFruit = getTheNearestFruit(animal);
	
	if (theNearestFruit)
	{
		inputsForBrain.push_back(
			static_cast<Blueberry::Scalar>(
				theNearestFruit->getPosition().x - animal.getPosition().x
			)
		);
		inputsForBrain.push_back(
			static_cast<Blueberry::Scalar>(
				theNearestFruit->getPosition().y - animal.getPosition().y
			)
		);
	}
	else
	{
		inputsForBrain.push_back(0.0);
		inputsForBrain.push_back(0.0);
	}

	return inputsForBrain;
}

const Fruit* Ecosystem::getTheNearestFruit(const Animal& animal) const
{
	if (m_fruits.empty()) return nullptr;
	
	int theNearestFruitIndex = -1;
	float theSmallestDistance = INFINITY;

	for (int i = 0; i < m_fruits.size(); i++)
	{
		float distance = calcDistance(animal, *m_fruits[i]);

		if (distance < theSmallestDistance)
		{
			theSmallestDistance = distance;
			theNearestFruitIndex = i;
		}
	}

	return m_fruits[theNearestFruitIndex].get();
}

float Ecosystem::calcDistance(const Animal& animal, const Fruit& fruit) const
{
	float x = animal.getPosition().x - fruit.getPosition().x;
	float y = animal.getPosition().y - fruit.getPosition().y;

	return sqrt(pow(x, 2) + pow(y, 2));
}

void Ecosystem::transferEnergyFromAnimalsToFruits()
{
	Fruit* lowestEnergyFruit = getLowestEnergyFruit();

	for (const auto& animal : m_animals)
	{
		lowestEnergyFruit->setEnergy(
			lowestEnergyFruit->getEnergy()
			+ animal->getEnergyToExpel()
		);
	}
}

Fruit* Ecosystem::getLowestEnergyFruit()
{
	if (m_fruits.empty()) return nullptr;

	Fruit* lowestEnergyFruit = m_fruits[0].get();

	for (const auto& fruit : m_fruits)
	{
		if (fruit->getEnergy() < lowestEnergyFruit->getEnergy())
		{
			lowestEnergyFruit = fruit.get();
		}
	}

	return lowestEnergyFruit;
}

void Ecosystem::avoidTunneling()
{
	for (auto& animal : m_animals)
	{
		// it is possible that an animal crossed 2 perpendicular borders 
		// in the same frame, 
		// so we have to call both of these functions: 
		avoidTunnelingByVerticalBorders(*animal);
		avoidTunnelingByHorizontalBorders(*animal);
	}
}

void Ecosystem::avoidTunnelingByVerticalBorders(Animal& animal)
{
	// make the following if statements shorter:
	const sf::Vector2f& animalPos = animal.getPosition();
	float animalRadius = animal.getRadius();
	float bordersThickness = getBordersThickness();
	const sf::Vector2f worldSize = getWorldSize();

	// left border:
	if (animalPos.x - animalRadius < bordersThickness)
	{
		animal.setVelocity(
			sf::Vector2i(
				abs(animal.getVelocityVector().x),
				animal.getVelocityVector().y
			)
		);
		animal.setPosition(
			sf::Vector2f(
				bordersThickness + animalRadius,
				animalPos.y
			)
		);
	}
	// right border (it's impossible to cross left and right border 
	// in the same frame so we can use "else if"):
	else if (animalPos.x + animalRadius > worldSize.x - bordersThickness)
	{
		animal.setVelocity(
			sf::Vector2i(
				-abs(animal.getVelocityVector().x),
				animal.getVelocityVector().y
			)
		);
		animal.setPosition(
			sf::Vector2f(
				worldSize.x - bordersThickness - animalRadius,
				animalPos.y
			)
		);
	}
}

void Ecosystem::avoidTunnelingByHorizontalBorders(Animal& animal)
{
	// let's make the following if statements shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	float animalRadius = animal.getRadius();
	float bordersThickness = getBordersThickness();
	const sf::Vector2f worldSize = getWorldSize();

	// top border:
	if (animalPos.y - animalRadius < bordersThickness)
	{
		animal.setVelocity(
			sf::Vector2i(
				animal.getVelocityVector().x,
				abs(animal.getVelocityVector().y)
			)
		);
		animal.setPosition(
			sf::Vector2f(
				animalPos.x,
				bordersThickness + animalRadius
			)
		);
	}
	// bottom border (it's impossible to cross top and bottom border 
	// in the same frame so we can use "else if"):
	else if (animalPos.y + animalRadius > worldSize.y - bordersThickness)
	{
		animal.setVelocity(
			sf::Vector2i(
				animal.getVelocityVector().x,
				-abs(animal.getVelocityVector().y)
			)
		);
		animal.setPosition(
			sf::Vector2f(
				animalPos.x,
				worldSize.y - bordersThickness - animalRadius
			)
		);
	}
}

void Ecosystem::kickInAssAnimalsStuckedNextToBorders()
{
	for (int i = 0; i < m_animals.size();)
	{
		if (sticksToBorder(*m_animals[i]))
		{
			convertAnimalToFruit(m_animals[i], true);
		}
		else i++;
	}
}

bool Ecosystem::sticksToBorder(const Animal& animal)
{
	if (sticksToLeftBorder(animal)) return true;
	
	if (sticksToRightBorder(animal)) return true;
	 
	if (sticksToTopBorder(animal)) return true;
	
	if (sticksToBottomBorder(animal)) return true;

	return false;
}

bool Ecosystem::sticksToLeftBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().x > 0.0f) return false;

	// let's make the following if statement shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	float bordersThickness = getBordersThickness();
	float animalRadius = animal.getRadius();

	return animalPos.x == bordersThickness + animalRadius;
}

bool Ecosystem::sticksToRightBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().x < 0.0f) return false;

	// let's make the following if statement shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	const sf::Vector2f& worldSize = getWorldSize();
	float bordersThickness = getBordersThickness();
	float animalR = animal.getRadius();

	return animalPos.x == worldSize.x - bordersThickness - animalR;
}

bool Ecosystem::sticksToTopBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().y > 0.0f) return false;

	// let's make the following if statement shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	float bordersThickness = getBordersThickness();
	float animalRadius = animal.getRadius();

	return animalPos.y == bordersThickness + animalRadius;
}

bool Ecosystem::sticksToBottomBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().y < 0.0f) return false;

	// let's make the following if statement shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	const sf::Vector2f& worldSize = getWorldSize();
	float bordersThickness = getBordersThickness();
	float animalR = animal.getRadius();

	return animalPos.y == worldSize.y - bordersThickness - animalR;
}

void Ecosystem::removeDeadAnimals()
{
	for (int i = 0; i < m_animals.size();)
	{
		if (!m_animals[i]->isAlive())
		{
			m_fruits.push_back(
				std::make_unique<Fruit>(
					m_animals[i]->getTotalEnergy(),
					m_animals[i]->getPosition(),
					m_fruitsRadius,
					m_fruitsColor
				)
			);

			std::cout
				<< "Rmved a dead animal 'cause of its hp="
				<< m_animals[i]->getHp() << '\n';

			removeAnimal(m_animals[i]);
		}
		else i++;
	}
}

void Ecosystem::removeAnimal(std::shared_ptr<Animal>& animal)
{
	if (animal.get() == m_trackedAnimal)
	{
		m_trackedAnimal = nullptr;
	}

	if (animal.get() == m_animalWithModifiedBrain)
	{
		m_animalWithModifiedBrain = nullptr;
	}
	
	std::swap(animal, m_animals.back());
	m_animals.pop_back();
}

void Ecosystem::feedAnimals(
	float dt,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	if (m_animals.empty()) return;

	std::sort(m_animals.begin(), m_animals.end(), compareAnimalsYPositions);

	for (int f = 0; f < m_fruits.size(); f++)
	{
		unsigned left = 0, right = m_animals.size() - 1U;

		while (left < right)
		{
			unsigned center = (left + right) / 2;

			if (animalIsTooHigh(*m_animals[center], *m_fruits[f]))
			{
				left = center + 1;
			}
			else
			{
				right = center;
			}
		}

		tryToFindConsumer(*m_fruits[f], left, dt, mousePos, events);
	}
}

bool Ecosystem::compareAnimalsYPositions(
	std::shared_ptr<Animal> a1,
	std::shared_ptr<Animal> a2)
{
	return a1->getPosition().y < a2->getPosition().y;
}

bool Ecosystem::animalIsTooHigh(const Animal& animal, const Fruit& fruit) const
{
	// let's make the following return statement shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	const sf::Vector2f& fruitPos = fruit.getPosition();
	float animalRadius = animal.getRadius();
	float fruitRadius = fruit.getRadius();

	return (animalPos.y - fruitPos.y) < -(animalRadius + fruitRadius);
}

int Ecosystem::tryToFindConsumer(
	Fruit& fruit, 
	unsigned startAnimalIndex, 
	float dt,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	unsigned animal_index = startAnimalIndex;

	while (animal_index < m_animals.size() 
		   && animalReachesFruitInY(*m_animals[animal_index], fruit))
	{
		if (animalReachesFruit(*m_animals[animal_index], fruit))
		{
			eat(*m_animals[animal_index], fruit, dt, mousePos, events);
			return 1;
		}

		animal_index++;
	}

	return 0;
}

bool Ecosystem::animalReachesFruitInY(
	const Animal& animal, 
	const Fruit& fruit) const
{
	// let's make the following return statement shorter:

	const sf::Vector2f& animalPos = animal.getPosition();
	const sf::Vector2f& fruitPos = fruit.getPosition();
	float animalRadius = animal.getRadius();
	float fruitRadius = fruit.getRadius();

	return abs(animalPos.y - fruitPos.y) <= animalRadius + fruitRadius;
}

bool Ecosystem::animalReachesFruit(
	const Animal& animal, 
	const Fruit& fruit) const
{
	float x = animal.getPosition().x - fruit.getPosition().x;
	float y = animal.getPosition().y - fruit.getPosition().y;

	float distance = sqrt(pow(x, 2) + pow(y, 2));

	return distance <= animal.getRadius() + fruit.getRadius();
}

void Ecosystem::eat(
	Animal& animal, 
	Fruit& fruit, 
	float dt,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	unsigned fps = static_cast<unsigned>(1.0f / dt);

	if (fps < 30U)
	{
		animal.setHp(animal.getHp() + fruit.getEnergy());
		fruit.setEnergy(0U);

		// TODO: a good candidate for logging:
		//std::cout << "FPS to low to clone!\n";

		return;
	}

	unsigned prevAnimalHp = animal.getHp();

	animal.setHp(
		std::min(
			prevAnimalHp + fruit.getEnergy(),
			m_defaultAnimalsHp
		)
	);

	std::cout << "prev fruit energy: " << fruit.getEnergy() << '\n';

	fruit.setEnergy(fruit.getEnergy() + prevAnimalHp - animal.getHp());

	std::cout << "prevAnimalHp: " << prevAnimalHp << '\n';
	std::cout << "fruit energy: " << fruit.getEnergy() << '\n';
	std::cout << "animal hp: " << animal.getHp() << '\n';
	std::cout << "m_defaultAnimalHp: " << m_defaultAnimalsHp << '\n';

	if (animal.getTotalEnergy() == 0)
	{
		std::cerr << "Animal total energy = 0\n";
		exit(-13);
	}

	// there is no more energy left in the fruit after eating:
	if (fruit.getEnergy() == 0) return;

	// if there is any energy left in the fruit make animal clone!:
	// TODO: put the following lines of code into a separate method
	m_animals.push_back(std::make_shared<Animal>(animal));
	
	m_animals.back()->setHp(fruit.getEnergy());
	m_animals.back()->setVelocity(sf::Vector2i(0, 0));
	
	if (m_animals.back()->getTotalEnergy() <= 0)
	{
		std::cerr << "m_animals.back()->getTotalEnergy() <= 0\n";
		std::cerr
			<< m_animals.back()->getHp() << ' '
			<< m_animals.back()->getKineticEnergy();
		exit(-13);
	}
	
	m_animals.back()->randomMutate(m_mutationsPerMutation, mousePos, events);

	if (&animal == m_trackedAnimal)
	{
		m_animals.back()->setColor(m_animalsColor);
	}

	m_hpBarsVisibility[m_animals.back().get()]
		= m_hpBarsVisibility[&animal];

	m_brainsVisibility[m_animals.back().get()]
		= m_brainsVisibility[&animal];

	fruit.setEnergy(0);
	
	//if (fruit.getEnergy() + animal.getHp() > animal.getMaxHp())
	//{
	//	fruit.setEnergy(fruit.getEnergy() + animal.getHp() - animal.getMaxHp());
	//	animal.setHp(animal.getMaxHp());
	//
	//	// clone using energy surplus!:
	//	while (fruit.getEnergy() > animal.getMaxHp())
	//	{
	//		fruit.setEnergy(fruit.getEnergy() - animal.getMaxHp());
	//
	//		m_animals.push_back(std::make_shared<Animal>(animal));
	//
	//		m_animals.back()->setHp(animal.getMaxHp());
	//		m_animals.back()->setVelocity(sf::Vector2f(0.0f, 0.0f));
	//		m_animals.back()->randomMutate(m_mutationsPerMutation);
	//
	//		if (&animal == m_trackedAnimal)
	//		{
	//			m_animals.back()->setColor(m_animalsColor);
	//		}
	//
	//		m_hpBarsVisibility[m_animals.back().get()]
	//			= m_hpBarsVisibility[&animal];
	//
	//		m_brainsVisibility[m_animals.back().get()]
	//			= m_brainsVisibility[&animal];
	//	}
	//
	//	m_animals.push_back(std::make_shared<Animal>(animal));
	//
	//	m_animals.back()->setHp(fruit.getEnergy());
	//	m_animals.back()->setVelocity(sf::Vector2f(0.0f, 0.0f));
	//	m_animals.back()->randomMutate(m_mutationsPerMutation);
	//
	//	if (&animal == m_trackedAnimal)
	//	{
	//		m_animals.back()->setColor(m_animalsColor);
	//	}
	//
	//	m_hpBarsVisibility[m_animals.back().get()] 
	//		= m_hpBarsVisibility[&animal];
	//
	//	m_brainsVisibility[m_animals.back().get()] 
	//		= m_brainsVisibility[&animal];
	//}
	//else
	//{
	//	animal.increaseHp(fruit.getEnergy());
	//}
	//
	//fruit.setEnergy(0.0);
}

void Ecosystem::removeEatenFruits()
{
	for (int i = 0; i < m_fruits.size();)
	{
		if (m_fruits[i]->getEnergy() == 0)
		{
			removeFruit(m_fruits[i]);
		}
		else i++;
	}
}

void Ecosystem::removeFruit(std::unique_ptr<Fruit>& fruit)
{
	std::swap(fruit, m_fruits.back());
	m_fruits.pop_back();
}

void Ecosystem::setHpBarsRanges()
{
	int theBiggestHp = getTheBiggestHp();
	
	for (auto& animal : m_animals)
	{
		animal->setHpBarRange(
			{
				animal->getHpBar()->getValuesRange().x,
				theBiggestHp
			}
		);
	}
}

unsigned Ecosystem::getTheBiggestHp() const
{
	unsigned theBiggestHp = -INFINITY;

	for (const auto& animal : m_animals)
	{
		// avoid a dangling reference:
		const unsigned temp = animal->getHp();
		theBiggestHp = std::max(theBiggestHp, temp);
	}

	return theBiggestHp;
}

void Ecosystem::correctPopulationSize(float dt)
{
	if (m_animals.empty()) return;

	float fastingThreshold = 5'000.0f / m_animals.size();

	//std::cout 
	//	<< "starvation death threshold: " 
	//	<< fastingThreshold 
	//	<< " sec\n";

	randomShuffleAnimals();

	std::sort(
		m_animals.begin(),
		m_animals.end(),
		correctPopulationSizeComparator
	);

	if (m_animals[0]->getTimeElapsedSinceLastExternalHpChange()
		< m_animals[1]->getTimeElapsedSinceLastExternalHpChange())
	{
		std::cerr << "wrong time elapsed\n";
		exit(-13);
	}

	unsigned murdersCount = 0U;

	for (int i = 0; i < m_animals.size() / 2; i++)
	{
		if (m_animals[i]->getTimeElapsedSinceLastExternalHpChange()
			> fastingThreshold)
		{
			murdersCount++;
		}
		else break;
	}

	// convert them into fruits:
	for (int i = 0; i < murdersCount; i++)
	{
		//std::cout << m_animals[i]->getTimeElapsedSinceLastExternalHpChange() << '\n';
		convertAnimalToFruit(m_animals[i], false);
	}

	//if (murdersCount != 0U)
	//{
	//	std::cout 
	//		<< "\npopulation corrected by " 
	//		<< murdersCount << "\n\n";
	//}

	//unsigned prevAnimalsCount = m_animals.m_size();
	//
	//std::sort(
	//	m_animals.begin(),
	//	m_animals.end(),
	//	correctPopulationSizeComparator
	//);
	//
	//assert(m_animals[0]->getTimeElapsedSinceLastExternalHpChange()
	//	>= m_animals[1]->getTimeElapsedSinceLastExternalHpChange());
	//
	//unsigned fps = 1U / dt;
	//unsigned murdersCount = 0U;
	//
	////std::cout << "fps: " << fps << '\n';
	//
	//if (fps < 10U)
	//{
	//	murdersCount = 0.5f * m_animals.m_size();
	//}
	//else if (fps < 30 && fps >= 10U)
	//{
	//	murdersCount = 0.1f * m_animals.m_size();
	//}
	//
	//while (murdersCount--)
	//{
	//	std::cout
	//		<< "kill: "
	//		<< m_animals[0]->getTimeElapsedSinceLastExternalHpChange()
	//		<< '\n';
	//	convertAnimalToFruit(m_animals[0], false);
	//}
	//
	//std::cout << '\n';
	//
	//// now kill animals starving for more than 1 minute:
	//for (int i = 0; i < m_animals.m_size();)
	//{
	//	if (2U * m_animals.m_size() <= prevAnimalsCount) return;
	//
	//	// TODO: "unhardcode" that:
	//	if (m_animals[i]->getTimeElapsedSinceLastExternalHpChange() > 60.0f)
	//	{
	//		convertAnimalToFruit(m_animals[i], false);
	//	}
	//	else i++;
	//}
	//
	//assert(2U * (m_animals.m_size() + 1) > prevAnimalsCount);
}

void Ecosystem::randomShuffleAnimals()
{
	unsigned animalsSize = m_animals.size();

	for (int i = 0; i < animalsSize; i++)
	{
		unsigned randIdx = Blueberry::RandomEngine::getIntInRange(
			i, 
			animalsSize - 1
		);
		std::swap(m_animals[i], m_animals[randIdx]);
	}
}

bool Ecosystem::correctPopulationSizeComparator(
	std::shared_ptr<Animal> a1, 
	std::shared_ptr<Animal> a2)
{
	return a1->getTimeElapsedSinceLastExternalHpChange()
		   > a2->getTimeElapsedSinceLastExternalHpChange();
}

void Ecosystem::correctBrainPreviewsPositions()
{
	for (auto& animal : m_animals)
	{
		// put the following iteration into a method?:

		const gui::BrainPreview& brainPreview = animal->getBrainPreview();

		bool protrudesRightBorder = brainPreviewProtrudesWorldRightBorder(
			brainPreview
		);
		bool protrudesBottomBorder = brainPreviewProtrudesWorldBottomBorder(
			brainPreview
		);

		// let's make the following lines of code shorter:

		if (protrudesRightBorder)
		{
			animal->setBrainPreviewPosition(
				animal->getPosition().x - brainPreview.getSize().x,
				brainPreview.getPosition().y
			);
		}
		else
		{
			animal->setBrainPreviewPosition(
				animal->getPosition().x,
				brainPreview.getPosition().y
			);
		}

		if (protrudesBottomBorder)
		{
			animal->setBrainPreviewPosition(
				brainPreview.getPosition().x,
				animal->getPosition().y - brainPreview.getSize().y
			);
		}
		else
		{
			animal->setBrainPreviewPosition(
				brainPreview.getPosition().x,
				animal->getPosition().y
			);
		}
	}
}

bool Ecosystem::brainPreviewProtrudesWorldRightBorder(
	const gui::BrainPreview& brainPreview)
{
	float rightBorderPosition = brainPreview.getPosition().x;
	rightBorderPosition += brainPreview.getSize().x;

	return rightBorderPosition > getWorldSize().x;
}

bool Ecosystem::brainPreviewProtrudesWorldBottomBorder(
	const gui::BrainPreview& brainPreview)
{
	float bottomBorderPosition = brainPreview.getPosition().y;
	bottomBorderPosition += brainPreview.getSize().y;

	return bottomBorderPosition > getWorldSize().y;
}

void Ecosystem::correctFruitsCount()
{
	#if _DEBUG
	unsigned idx1 = 0U;
	#endif

	for (int i = 0; i < m_fruits.size(); i++)
	{
		#if _DEBUG
		idx1++;
		#endif

		Fruit* fruit = m_fruits[i].get();

		if (fruit->getEnergy() >= 2 * m_defaultFruitEnergy)
		{
			unsigned prevEnergy = fruit->getEnergy();

			m_fruits.emplace_back(
				std::make_unique<Fruit>(
					fruit->getEnergy() - m_defaultFruitEnergy,
					fruit->getPosition(),
					fruit->getRadius(),
					fruit->getColor()
				)
			);
			m_fruits.back()->setRandomPosition(
				getWorldSize(), 
				getBordersThickness(),
				0.1f // unhardcode margins
			);

			fruit->setEnergy(m_defaultFruitEnergy);

			if (prevEnergy
				!= fruit->getEnergy() + m_fruits.back()->getEnergy())
			{
				std::cerr << "BUG IN: void Ecosystem::correctFruitsCount()\n";
				exit(-13);
			}
		}

		if (fruit->getEnergy() >= 2 * m_defaultFruitEnergy)
		{
			std::cerr << "BUG IN: void Ecosystem::correctFruitsCount()\n";
			exit(-14);
		}
	}

	#if _DEBUG
	assert(idx1 == m_fruits.size());
	#endif

	#if _DEBUG
	for (const auto& fruit : m_fruits)
	{
		assert(fruit->getEnergy() < 2 * m_defaultFruitEnergy);
	}
	#endif
}

void Ecosystem::updateOnlyImgBtnsOfBrainsPreviews(
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	for (auto& animal : m_animals)
	{
		animal->updateOnlyImgBtnOfBrainPreview(mousePos, events);
	}
}

void Ecosystem::updateModifyingBrainsPreviews()
{
	for (const auto& animal : m_animals)
	{
		if (animal->getBrainPreview().getImgBtn()->hasBeenClicked())
		{
			m_animalWithModifiedBrain = animal.get();
			return;
		}
	}
}
