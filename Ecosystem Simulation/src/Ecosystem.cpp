#include "stdafx.h"
#include "Ecosystem.h"

Ecosystem::Ecosystem(
	const std::string& name,
	const sf::Vector2f& world_size,
	unsigned borders_thickness,
	const sf::Color& background_color,
	const sf::Color& borders_color,
	unsigned animals_count,
	unsigned fruits_count,
	float animals_radius,
	float fruits_radius,
	float default_animals_hp,
	float default_fruits_energy,
	float mutation_percentage,
	const sf::Color& animals_color,
	const sf::Color& fruits_color,
	const sf::Color& tracked_animal_color,
	float simulation_speed_factor,
	bool simulation_is_paused,
	GodTool god_tool,
	bool render_hp_bars_by_default,
	bool render_brains_by_default)
	: name(name),
	  mutationPercentage(mutation_percentage),
	  animalsRadius(animals_radius),
	  fruitsRadius(fruits_radius),
	  animalsColor(animals_color),
	  fruitsColor(fruits_color),
	  trackedAnimalColor(tracked_animal_color),
	  trackedAnimal(nullptr),
	  simulationSpeedFactor(simulation_speed_factor),
	  m_simulationIsPaused(simulation_is_paused),
	  godTool(god_tool),
	  totalTimeElapsed(0.0f)
{
	this->initBackgroundAndBorders(world_size, borders_thickness, background_color, borders_color);
	this->createNewAnimals(
		animals_count,
		default_animals_hp,
		animals_radius,
		animals_color,
		render_hp_bars_by_default,
		render_brains_by_default
	);
	this->createNewFruits(fruits_count, default_fruits_energy, fruits_radius, fruits_color);
}

Ecosystem::Ecosystem(const std::string& folder_path)
{
	this->loadFromFolder(folder_path);
}

// public methods:
void Ecosystem::saveToFolder(const std::string& folder_path) const
{
	// TODO: add static private (or sth even better) methods that returns those paths:
	this->saveAnimals(folder_path + "/animals");
	this->saveFruits(folder_path + "/fruits");
	this->saveEcosystem(folder_path + "/ecosystem.ini");
	this->saveHpBarsVisibility(folder_path + "/hp bars visibility.ini");
	this->saveBrainsPreviewsVisibility(folder_path + "/brains previews visibility.ini");
}

void Ecosystem::loadFromFolder(const std::string& folder_path)
{
	// TODO: add static private (or sth even better) methods that returns those paths:
	this->loadAnimals(folder_path + "/animals");
	this->loadFruits(folder_path + "/fruits");
	this->loadEcosystem(folder_path + "/ecosystem.ini");
	this->loadHpBarsVisibility(folder_path + "/hp bars visibility.ini");
	this->loadBrainsPreviewsVisibility(folder_path + "/brains previews visibility.ini");
}

void Ecosystem::useGodTools(
	const std::vector<sf::Event>& events,
	const sf::Vector2f& mouse_pos_view)
{
	if (!EventsAccessor::hasEventOccured(sf::Event::MouseButtonPressed, events))
		return;
	
	// now use God tool:
	switch (this->godTool)
	{
	case GodTool::NONE:
		return;

	case GodTool::TRACK:
		this->trackingTool(mouse_pos_view);
		return;

	case GodTool::KILL:
		this->killingTool(mouse_pos_view);
		return;

	case GodTool::REPLACE:
		this->replacingTool(mouse_pos_view);
		return;

	case GodTool::BRAIN:
		this->brainTool(mouse_pos_view);
		return;

	case GodTool::STOP:
		this->stoppingTool(mouse_pos_view);
		return;

	case GodTool::INFO:
		this->infoTool(mouse_pos_view);
		return;

	default:
		throw "ERROR::Ecosystem::useGodTool::ECOSYSTEM GOD TOOL IS NOT SUPPORTED BY THIS METHOD\n";
	}
}

void Ecosystem::update(float dt)
{
	if (!this->m_simulationIsPaused)
	{
		this->totalTimeElapsed += dt;
		this->updateWorld(dt);
	}
}

void Ecosystem::render(sf::RenderTarget& target) const
{
	target.draw(this->background);

	for (const auto& animal : this->animals)
		animal->renderBody(target);

	for (const auto& animal : this->animals)
		if (this->hpBarsVisibility.at(animal.get()))
			animal->renderHpBar(target);

	for (const auto& fruit : this->fruits)
		fruit->render(target);

	for (const auto& animal : this->animals)
		if (this->brainsPreviewsVisibility.at(animal.get()))
			animal->renderBrainPreview(target);
}

// accessors:
const std::string& Ecosystem::getName() const
{
	return this->name;
}

sf::Vector2f Ecosystem::getWorldSize() const
{
	sf::Vector2f result = {
		this->background.getSize().x + 2 * this->background.getOutlineThickness(),
		this->background.getSize().y + 2 * this->background.getOutlineThickness()
	};

	return result;
}

unsigned Ecosystem::getBordersThickness() const
{
	return this->background.getOutlineThickness();
}

unsigned Ecosystem::getAnimalsCount() const
{
	return this->animals.size();
}

const std::vector<std::shared_ptr<Animal>>& Ecosystem::getAnimals() const
{
	return this->animals;
}

unsigned Ecosystem::getFruitsCount() const
{
	return this->fruits.size();
}

float Ecosystem::getMutationPercentage() const
{
	return this->mutationPercentage;
}

const sf::Color& Ecosystem::getBackgroundColor() const
{
	return this->background.getFillColor();
}

const sf::Color& Ecosystem::getBordersColor() const
{
	return this->background.getOutlineColor();
}

const sf::Color& Ecosystem::getAnimalsColor() const
{
	return this->animalsColor;
}

const sf::Color& Ecosystem::getFruitsColor() const
{
	return this->fruitsColor;
}

const sf::Color& Ecosystem::getTrackedAnimalColor() const
{
	return this->trackedAnimalColor;
}

const Animal* Ecosystem::getTrackedAnimal() const
{
	return this->trackedAnimal;
}

float Ecosystem::getSimulationSpeedFactor() const
{
	return this->simulationSpeedFactor;
}

bool Ecosystem::isSimulationPaused() const
{
	return this->m_simulationIsPaused;
}

GodTool Ecosystem::getCurrentGodTool() const
{
	return this->godTool;
}

float Ecosystem::getTotalTimeElapsed() const
{
	return this->totalTimeElapsed;
}

float Ecosystem::getTotalAnimalsHpEnergy() const
{
	float totalHpEnergy = 0.0f;

	for (const auto& animal : this->animals)
		totalHpEnergy += animal->getHp();

	return totalHpEnergy;
}

float Ecosystem::getTotalAnimalsKineticEnergy() const
{
	float totalKineticEnergy = 0.0f;

	for (const auto& animal : this->animals)
		totalKineticEnergy += animal->getKineticEnergy();

	return totalKineticEnergy;
}

float Ecosystem::getTotalFruitsEnergy() const
{
	float totalFruitsEnergy = 0.0f;

	for (const auto& fruit : this->fruits)
		totalFruitsEnergy += fruit->getEnergy();

	return totalFruitsEnergy;
}

float Ecosystem::getTotalEnergy() const
{
	return this->getTotalAnimalsHpEnergy() + this->getTotalAnimalsKineticEnergy() + this->getTotalFruitsEnergy();
}

void Ecosystem::printAllAnimalsPositions() const
{
	for (const auto& animal : this->animals)
		std::cout << animal->getPosition().x << ' ' << animal->getPosition().y << '\n';
}

// mutators:
void Ecosystem::setName(const std::string& name)
{
	this->name = name;
}

void Ecosystem::setMutationPercentage(float mutation_rate)
{
	this->mutationPercentage = mutation_rate;
}

void Ecosystem::setBackgroundColor(const sf::Color& background_color)
{
	this->background.setFillColor(background_color);
}

void Ecosystem::setBordersColor(const sf::Color& borders_color)
{
	this->background.setOutlineColor(borders_color);
}

void Ecosystem::setAnimalsColor(const sf::Color& animals_color)
{
	this->animalsColor = animals_color;

	for (auto& animal : this->animals)
		if (animal.get() != this->trackedAnimal)
			animal->setColor(animals_color);
}

void Ecosystem::setFruitsColor(const sf::Color& fruits_color)
{
	this->fruitsColor = fruits_color;

	for (auto& fruit : this->fruits)
		fruit->setColor(fruits_color);
}

void Ecosystem::setTrackedAnimalColor(const sf::Color& tracked_animal_color)
{
	this->trackedAnimalColor = tracked_animal_color;

	if (this->trackedAnimal)
		this->trackedAnimal->setColor(tracked_animal_color);
}

void Ecosystem::setSimulationSpeedFactor(float simulation_speed_factor)
{
	this->simulationSpeedFactor = simulation_speed_factor;
}

void Ecosystem::pauseSimulation()
{
	this->m_simulationIsPaused = true;
}

void Ecosystem::unpauseSimulation()
{
	this->m_simulationIsPaused = false;
}

void Ecosystem::setGodTool(GodTool god_tool)
{
	this->godTool = god_tool;
}

// private methods:

// initialization:
void Ecosystem::initBackgroundAndBorders(
	const sf::Vector2f& world_size,
	unsigned borders_thickness,
	const sf::Color& background_color,
	const sf::Color& borders_color)
{
	this->background.setSize(
		sf::Vector2f(
			world_size.x - 2.f * borders_thickness,
			world_size.y - 2.f * borders_thickness
		)
	);
	this->background.setPosition(borders_thickness, borders_thickness);
	this->background.setFillColor(background_color);

	this->background.setOutlineThickness(borders_thickness);
	this->background.setOutlineColor(borders_color);
}

void Ecosystem::createNewAnimals(
	unsigned animals_count,
	float default_animals_hp,
	float animals_radius,
	const sf::Color& animals_color,
	bool render_hp_bars_by_default,
	bool render_brains_by_default
)
{
	this->animals.clear();

	for (int i = 0; i < animals_count; i++)
		this->createNewAnimal(
			default_animals_hp,
			animals_radius,
			animals_color,
			render_hp_bars_by_default,
			render_brains_by_default
		);
}

void Ecosystem::createNewAnimal(
	float default_animal_hp,
	float animal_radius,
	const sf::Color& animal_color,
	bool render_hp_bar_by_default,
	bool render_brain_by_default)
{
	this->animals.push_back(
		std::make_unique<Animal>(
			sf::Vector2f(
				this->getWorldSize().x / 2.0f,
				this->getWorldSize().y / 2.0f
			),
			animal_radius,
			animal_color,
			sf::Color(100, 100, 100),
			sf::Color::Red,
			default_animal_hp,
			default_animal_hp
		)
	);
	this->animals.back()->setRandomPosition(this->getWorldSize(), this->getBordersThickness());

	this->hpBarsVisibility[this->animals.back().get()] = render_hp_bar_by_default;
	this->brainsPreviewsVisibility[this->animals.back().get()] = render_brain_by_default;
}

void Ecosystem::createNewFruits(
	unsigned fruits_count,
	float default_fruits_energy,
	float fruits_radius,
	const sf::Color& fruits_color)
{
	this->fruits.clear();

	for (int i = 0; i < fruits_count; i++)
		this->createNewFruit(default_fruits_energy, fruits_radius, fruits_color);
}

void Ecosystem::createNewFruit(float energy, float radius, const sf::Color& fruit_color)
{
	this->fruits.push_back(
		std::make_unique<Fruit>(
			energy,
			sf::Vector2f(
				0.0f,
				0.0f
			),
			radius,
			fruit_color
		)
	);
	this->fruits.back()->setRandomPosition(this->getWorldSize(), this->getBordersThickness());
}

// private utilities:
void Ecosystem::saveAnimals(const std::string& folder_path) const
{
	for (int i = 0; i < this->animals.size(); i++)
		this->animals[i]->saveToFolder(folder_path + "/animals/animal" + std::to_string(i));
}

void Ecosystem::saveFruits(const std::string& folder_path) const
{
	for (int i = 0; i < this->fruits.size(); i++)
		this->fruits[i]->saveToFile(folder_path + "/fruits/fruit" + std::to_string(i) + ".ini");
}

void Ecosystem::saveEcosystem(const std::string& file_path) const
{
	std::ofstream ofs(file_path);

	if (!ofs.is_open())
	{
		std::cerr << "ERROR::saveToFolder::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	ofs << this->name << '\n';
	ofs << this->getWorldSize().x << ' ' << this->getWorldSize().y << '\n';
	ofs << this->background.getOutlineThickness() << '\n';

	ofs << static_cast<int>(this->background.getFillColor().r) << '\n';
	ofs << static_cast<int>(this->background.getFillColor().g) << '\n';
	ofs << static_cast<int>(this->background.getFillColor().b) << '\n';
	ofs << static_cast<int>(this->background.getFillColor().a) << '\n';

	ofs << static_cast<int>(this->background.getOutlineColor().r) << '\n';
	ofs << static_cast<int>(this->background.getOutlineColor().g) << '\n';
	ofs << static_cast<int>(this->background.getOutlineColor().b) << '\n';
	ofs << static_cast<int>(this->background.getOutlineColor().a) << '\n';

	ofs << this->mutationPercentage << '\n';

	ofs << static_cast<int>(this->animalsColor.r) << '\n';
	ofs << static_cast<int>(this->animalsColor.g) << '\n';
	ofs << static_cast<int>(this->animalsColor.b) << '\n';
	ofs << static_cast<int>(this->animalsColor.a) << '\n';

	ofs << static_cast<int>(this->fruitsColor.r) << '\n';
	ofs << static_cast<int>(this->fruitsColor.g) << '\n';
	ofs << static_cast<int>(this->fruitsColor.b) << '\n';
	ofs << static_cast<int>(this->fruitsColor.a) << '\n';

	ofs << static_cast<int>(this->trackedAnimalColor.r) << '\n';
	ofs << static_cast<int>(this->trackedAnimalColor.g) << '\n';
	ofs << static_cast<int>(this->trackedAnimalColor.b) << '\n';
	ofs << static_cast<int>(this->trackedAnimalColor.a) << '\n';

	ofs << this->getTrackedAnimalIndex() << '\n';

	ofs << this->simulationSpeedFactor << '\n';
	ofs << this->m_simulationIsPaused << '\n';
	ofs << this->godTool << '\n';
	ofs << this->totalTimeElapsed;

	ofs.close();
}

void Ecosystem::saveHpBarsVisibility(const std::string& file_path) const
{
	std::ofstream ofs(file_path);

	if (ofs.is_open())
	{
		std::cerr << "ERROR::Ecosystem::saveHpBarsVisibility::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	for (const auto& it : this->hpBarsVisibility)
		ofs << it.second << '\n';

	ofs.close();
}

void Ecosystem::saveBrainsPreviewsVisibility(const std::string& file_path) const
{
	std::ofstream ofs(file_path);

	if (ofs.is_open())
	{
		std::cerr << "ERROR::Ecosystem::saveBrainsPreviewsVisibility::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	for (const auto& it : this->brainsPreviewsVisibility)
		ofs << it.second << '\n';

	ofs.close();
}

void Ecosystem::loadAnimals(const std::string& folder_path)
{
	this->animals.clear();

	for (const auto& entry : std::filesystem::directory_iterator(folder_path))
		this->loadAnimal(entry.path().string());
}

void Ecosystem::loadAnimal(const std::string& folder_path)
{
	this->animals.push_back(std::make_unique<Animal>(folder_path));
}

void Ecosystem::loadFruits(const std::string& folder_path)
{
	this->fruits.clear();

	for (const auto& entry : std::filesystem::directory_iterator(folder_path))
		this->loadFruit(entry.path().string());
}

void Ecosystem::loadFruit(const std::string& file_path)
{
	this->fruits.push_back(std::make_unique<Fruit>(file_path));
}

void Ecosystem::loadEcosystem(const std::string& file_path)
{
	std::ifstream ifs(file_path);

	if (!ifs.is_open())
	{
		std::cerr << "ERROR::Ecosystem::loadEcosystem::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	sf::Vector2f worldSize;
	float bordersThickness;
	sf::Color backgroundColor;
	sf::Color bordersColor;
	int trackedAnimalIndex;

	ifs >> this->name;
	ifs >> worldSize.x >> worldSize.y;
	ifs >> bordersThickness;

	ifs >> backgroundColor.r >> backgroundColor.g >> backgroundColor.b >> backgroundColor.a;
	ifs >> bordersColor.r >> bordersColor.g >> bordersColor.b >> bordersColor.a;

	ifs >> trackedAnimalIndex;

	ifs >> this->animalsColor.r >> this->animalsColor.g >> this->animalsColor.b >> this->animalsColor.a;
	ifs >> this->fruitsColor.r >> this->fruitsColor.g >> this->fruitsColor.b >> this->fruitsColor.a;
	ifs >> this->trackedAnimalColor.r >> this->trackedAnimalColor.g >> this->trackedAnimalColor.b >> this->trackedAnimalColor.a;

	ifs >> this->simulationSpeedFactor;
	ifs >> this->m_simulationIsPaused;
	ifs >> this->godTool;
	ifs >> this->totalTimeElapsed;

	this->initBackgroundAndBorders(worldSize, bordersThickness, backgroundColor, bordersColor);

	this->trackedAnimal = this->animals[trackedAnimalIndex].get();
	this->animals[trackedAnimalIndex]->setColor(this->trackedAnimalColor);

	ifs.close();
}

void Ecosystem::loadHpBarsVisibility(const std::string& file_path)
{
	std::ifstream ifs(file_path);

	if (!ifs.is_open())
	{
		std::cerr << "ERROR::Ecosystem::loadHpBarsVisibility::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	bool hpBarVisibility;

	for (int i = 0; i < this->animals.size(); i++)
	{
		ifs >> hpBarVisibility;
		this->hpBarsVisibility.at(this->animals[i].get()) = hpBarVisibility;
	}

	ifs.close();
}

void Ecosystem::loadBrainsPreviewsVisibility(const std::string& file_path)
{
	std::ifstream ifs(file_path);

	if (!ifs.is_open())
	{
		std::cerr << "ERROR::Ecosystem::loadBrainsPreviewsVisibility::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	bool brainPreviewVisibility;

	for (int i = 0; i < this->animals.size(); i++)
	{
		ifs >> brainPreviewVisibility;
		this->brainsPreviewsVisibility.at(this->animals[i].get()) = brainPreviewVisibility;
	}

	ifs.close();
}

int Ecosystem::getTrackedAnimalIndex() const
{
	if (!this->trackedAnimal)
		return -1;

	for (int i = 0; i < this->animals.size(); i++)
		if (this->animals[i].get() == this->trackedAnimal)
			return i;

	return -1;
}

// god tools:

// tracking tool:
void Ecosystem::trackingTool(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
	{
		if (animal->isCoveredByMouse(mouse_pos_view))
		{
			// the tracked animal is no longer tracked:
			if (animal.get() == this->trackedAnimal)
			{
				animal->setColor(this->animalsColor); // reset its color
				this->trackedAnimal = nullptr;
				return;
			}

			// a new animal is tracked:
			// reset body color of the previous tracked animal:
			if (this->trackedAnimal)
				this->trackedAnimal->setColor(this->animalsColor);

			// set new tracked animal:
			this->trackedAnimal = animal.get();

			// change color:
			animal->setColor(this->trackedAnimalColor);

			return;
		}
	}
}

// killing tool:
void Ecosystem::killingTool(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
		if (animal->isCoveredByMouse(mouse_pos_view))
		{
			this->convertAnimalToFruit(animal, false);
			return;
		}
}

void Ecosystem::convertAnimalToFruit(std::shared_ptr<Animal>& animal, bool random_fruit_position)
{
	this->fruits.push_back(
		std::make_unique<Fruit>(
			animal->getTotalEnergy(),
			animal->getPosition(),
			this->fruitsRadius,
			this->fruitsColor
		)
	);

	if (random_fruit_position)
		this->fruits.back()->setRandomPosition(this->getWorldSize(), this->getBordersThickness());

	if (animal.get() == this->trackedAnimal)
		this->trackedAnimal = nullptr;

	this->removeAnimal(animal);
}

// replacing tool:
void Ecosystem::replacingTool(const sf::Vector2f& mouse_pos_view)
{
	std::cout << "REPLACING TOOL IS NOT DEFINED YET!\n";
}

// brain tool:
void Ecosystem::brainTool(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
	{
		if (animal->isCoveredByMouse(mouse_pos_view))
		{
			this->brainsPreviewsVisibility[animal.get()] = !this->brainsPreviewsVisibility[animal.get()];
			return;
		}
	}
}

// stopping tool:
void Ecosystem::stoppingTool(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
	{
		if (animal->isCoveredByMouse(mouse_pos_view))
		{
			this->convertKineticEnergyToFruit(*animal, true);
			return;
		}
	}
}

void Ecosystem::convertKineticEnergyToFruit(Animal& animal, bool random_fruit_position)
{
	if (animal.getKineticEnergy() <= 0.0f)
		return;

	this->fruits.push_back(
		std::make_unique<Fruit>(
			animal.getKineticEnergy(),
			animal.getPosition(),
			this->fruitsRadius,
			this->fruitsColor
		)
	);

	if (random_fruit_position)
		this->fruits.back()->setRandomPosition(this->getWorldSize(), this->getBordersThickness());

	animal.setVelocity(sf::Vector2f(0.0f, 0.0f));
}

// info tool:
void Ecosystem::infoTool(const sf::Vector2f& mouse_pos_view) const
{
	bool noInfoPrinted = true;

	for (const auto& animal : this->animals)
		if (animal->isCoveredByMouse(mouse_pos_view))
		{
			noInfoPrinted = false;
			this->printInfoAboutAnimal(*animal);
		}		

	for (const auto& fruit : this->fruits)
		if (fruit->isCoveredByMouse(mouse_pos_view))
		{
			noInfoPrinted = false;
			this->printInfoAboutFruit(*fruit);
		}

	if (noInfoPrinted)
		this->printInfoAboutEcosystem();
}

void Ecosystem::printInfoAboutAnimal(const Animal& animal) const
{
	std::cout << '\n';
	std::cout << "Info about a clicked animal:\n";
	std::cout << "max hp: " << animal.getMaxHp() << '\n';
	std::cout << "position: " << animal.getPosition().x << ' ' << animal.getPosition().y << '\n';
	std::cout << "velocity: " << animal.getVelocityVector().x << ' ' << animal.getVelocityVector().y << '\n';
	std::cout << "acceleration: " << animal.getAccelerationVector().x << ' ' << animal.getAccelerationVector().y << '\n';
	std::cout << "kinetic energy: " << animal.getKineticEnergy() << '\n';
	std::cout << "HP: " << animal.getHp() << '\n';
	std::cout << "total energy: " << animal.getTotalEnergy() << '\n';
}

void Ecosystem::printInfoAboutFruit(const Fruit& fruit) const
{
	std::cout << '\n';
	std::cout << "Info about a clicked fruit:\n";
	std::cout << "energy: " << fruit.getEnergy() << '\n';
	std::cout << "position: " << fruit.getPosition().x << ' ' << fruit.getPosition().y << '\n';
}

void Ecosystem::printInfoAboutEcosystem() const
{
	std::cout << '\n';
	std::cout << "Info about the ecosystem:\n";
	std::cout << "name: " << this->name << '\n';
	std::cout << "arena size: "
		<< this->getWorldSize().x - 2U * this->getBordersThickness() << ' '
		<< this->getWorldSize().y - 2U * this->getBordersThickness() << '\n';
	std::cout << "borders thickness: " << this->getBordersThickness() << '\n';
	std::cout << "world size: " << this->getWorldSize().x << ' ' << this->getWorldSize().y << '\n';
	std::cout << "animals count: " << this->animals.size() << '\n';
	std::cout << "fruits count: " << this->fruits.size() << '\n';
	std::cout << "mutation rate percentage: " << this->mutationPercentage << '\n';
	std::cout << "simulation speed factor: " << this->simulationSpeedFactor << '\n';
	std::cout << "simulation is paused: " << this->m_simulationIsPaused << '\n';
	std::cout << "god tool: " << getGodToolStr(this->godTool) << '\n';
	std::cout << "total time elapsed [sec]: " << this->totalTimeElapsed << '\n';
	std::cout << "total animals hp energy: " << this->getTotalAnimalsHpEnergy() << '\n';
	std::cout << "total animals kinetic energy: " << this->getTotalAnimalsKineticEnergy() << '\n';
	std::cout << "total fruits energy: " << this->getTotalFruitsEnergy() << '\n';
	std::cout << "total energy: " << this->getTotalEnergy() << '\n';
}

// other private methods:
void Ecosystem::updateWorld(float dt)
{
	this->updateAnimals(dt);
	this->avoidTunneling();
	this->killAnimalsStickingToBorders();
	this->transferEnergyFromAnimalsToFruits();
	this->removeDeadAnimals();
	this->feedAnimals();
	this->removeEatenFruits();
	this->correctBrainPreviewsPositions();
}

void Ecosystem::updateAnimals(float dt)
{
	for (const auto& animal : this->animals)
		animal->update(dt, this->simulationSpeedFactor, this->getInputsForBrain(*animal));
}

const std::vector<CrappyNeuralNets::Scalar> Ecosystem::getInputsForBrain(const Animal& animal) const
{
	// TODO: add set BRAIN inputs methods in Animal class
	std::vector<CrappyNeuralNets::Scalar> inputsForBrain;

	inputsForBrain.reserve(5);

	inputsForBrain.push_back(animal.getVelocityVector().x);
	inputsForBrain.push_back(animal.getVelocityVector().y);
	
	inputsForBrain.push_back(log2(animal.getHp()));

	const Fruit* theNearestFruit = this->getTheNearestFruit(animal);
	
	if (theNearestFruit)
	{
		inputsForBrain.push_back(theNearestFruit->getPosition().x - animal.getPosition().x);
		inputsForBrain.push_back(theNearestFruit->getPosition().y - animal.getPosition().y);
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
	if (this->fruits.empty())
		return nullptr;
	
	int theNearestFruitIndex = -1;
	float theSmallestDistance = INFINITY;

	for (int i = 0; i < this->fruits.size(); i++)
	{
		float distance = this->calcDistance(animal, *this->fruits[i]);

		if (distance < theSmallestDistance)
		{
			theSmallestDistance = distance;
			theNearestFruitIndex = i;
		}
	}

	return this->fruits[theNearestFruitIndex].get();
}

float Ecosystem::calcDistance(const Animal& animal, const Fruit& fruit) const
{
	float x = animal.getPosition().x - fruit.getPosition().x;
	float y = animal.getPosition().y - fruit.getPosition().y;

	return sqrt(pow(x, 2) + pow(y, 2));
}

void Ecosystem::avoidTunneling()
{
	for (auto& animal : this->animals)
	{
		// it is possible that an animal crossed 2 perpendicular borders in the same frame, 
		// so we have to call both of these functions: 
		this->avoidTunnelingByVerticalBorders(*animal);
		this->avoidTunnelingByHorizontalBorders(*animal);
	}
}

void Ecosystem::avoidTunnelingByVerticalBorders(Animal& animal)
{
	// left border:
	if (animal.getPosition().x - animal.getRadius() < this->getBordersThickness())
	{
		animal.setVelocity(
			sf::Vector2f(
				abs(animal.getVelocityVector().x),
				animal.getVelocityVector().y
			)
		);
		animal.setPosition(
			sf::Vector2f(
				this->getBordersThickness() + animal.getRadius(),
				animal.getPosition().y
			)
		);
	}
	// right border (it's impossible to cross left and right border in the same frame so we can use "else if"):
	else if (animal.getPosition().x + animal.getRadius() > this->getWorldSize().x - this->getBordersThickness())
	{
		animal.setVelocity(
			sf::Vector2f(
				-abs(animal.getVelocityVector().x),
				animal.getVelocityVector().y
			)
		);
		animal.setPosition(
			sf::Vector2f(
				this->getWorldSize().x - this->getBordersThickness() - animal.getRadius(),
				animal.getPosition().y
			)
		);
	}
}

void Ecosystem::avoidTunnelingByHorizontalBorders(Animal& animal)
{
	// top border:
	if (animal.getPosition().y - animal.getRadius() < this->getBordersThickness())
	{
		animal.setVelocity(
			sf::Vector2f(
				animal.getVelocityVector().x,
				abs(animal.getVelocityVector().y)
			)
		);
		animal.setPosition(
			sf::Vector2f(
				animal.getPosition().x,
				this->getBordersThickness() + animal.getRadius()
			)
		);
	}
	// bottom border (it's impossible to cross top and bottom border in the same frame so we can use "else if"):
	else if (animal.getPosition().y + animal.getRadius() > this->getWorldSize().y - this->getBordersThickness())
	{
		animal.setVelocity(
			sf::Vector2f(
				animal.getVelocityVector().x,
				-abs(animal.getVelocityVector().y)
			)
		);
		animal.setPosition(
			sf::Vector2f(
				animal.getPosition().x,
				this->getWorldSize().y - this->getBordersThickness() - animal.getRadius()
			)
		);
	}
}

void Ecosystem::killAnimalsStickingToBorders()
{
	for (int i = 0; i < this->animals.size();)
	{
		if (this->sticksToBorder(*this->animals[i]))
			this->convertAnimalToFruit(this->animals[i], true);
		else
			i++;
	}
}

bool Ecosystem::sticksToBorder(const Animal& animal)
{
	if (this->sticksToLeftBorder(animal))
		return true;
	
	if (this->sticksToRightBorder(animal))
		return true;
	 
	if (this->sticksToTopBorder(animal))
		return true;
	
	if (this->sticksToBottomBorder(animal))
		return true;

	return false;
}

bool Ecosystem::sticksToLeftBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().x > 0.0f)
		return false;

	if (animal.getPosition().x != this->getBordersThickness() + animal.getRadius())
		return false;

	return true;
}

bool Ecosystem::sticksToRightBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().x < 0.0f)
		return false;

	if (animal.getPosition().x != this->getWorldSize().x - this->getBordersThickness() - animal.getRadius())
		return false;

	return true;
}

bool Ecosystem::sticksToTopBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().y > 0.0f)
		return false;

	if (animal.getPosition().y != this->getBordersThickness() + animal.getRadius())
		return false;

	return true;
}

bool Ecosystem::sticksToBottomBorder(const Animal& animal)
{
	if (animal.getAccelerationVector().y < 0.0f)
		return false;

	if (animal.getPosition().y != this->getWorldSize().y - this->getBordersThickness() - animal.getRadius())
		return false;

	return true;
}

void Ecosystem::removeDeadAnimals()
{
	for (int i = 0; i < this->animals.size();)
	{
		if (!this->animals[i]->isAlive())
		{
			this->fruits.push_back(
				std::make_unique<Fruit>(
					this->animals[i]->getTotalEnergy(),
					this->animals[i]->getPosition(),
					this->fruitsRadius,
					this->fruitsColor
					)
			);

			this->removeAnimal(this->animals[i]);
		}
		else
			i++;
	}
}

void Ecosystem::removeAnimal(std::shared_ptr<Animal>& animal)
{
	if (animal.get() == this->trackedAnimal)
		this->trackedAnimal = nullptr;
	
	std::swap(animal, this->animals.back());
	this->animals.pop_back();
}

void Ecosystem::feedAnimals()
{
	if (this->animals.empty())
		return;

	std::sort(this->animals.begin(), this->animals.end(), compareAnimalsYPositions);

	for (int f = 0; f < this->fruits.size(); f++)
	{
		unsigned left = 0, right = this->animals.size() - 1U;

		while (left < right)
		{
			unsigned center = (left + right) / 2;

			if (this->animalIsTooHigh(*this->animals[center], *this->fruits[f]))
			{
				left = center + 1;
			}
			else
			{
				right = center;
			}
		}

		this->tryToFindConsumer(*this->fruits[f], left);
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
	return (animal.getPosition().y - fruit.getPosition().y) < -(animal.getRadius() + fruit.getRadius());
}

int Ecosystem::tryToFindConsumer(Fruit& fruit, unsigned start_animal_index)
{
	unsigned animal_index = start_animal_index;

	while (animal_index < this->animals.size() && this->animalReachesFruitInY(*this->animals[animal_index], fruit))
	{
		if (this->animalReachesFruit(*this->animals[animal_index], fruit))
		{
			this->eat(*this->animals[animal_index], fruit);
			return 1;
		}
		animal_index++;
	}

	return 0;
}

bool Ecosystem::animalReachesFruitInY(const Animal& animal, const Fruit& fruit) const
{
	return abs(animal.getPosition().y - fruit.getPosition().y) <= animal.getRadius() + fruit.getRadius();
}

bool Ecosystem::animalReachesFruit(const Animal& animal, const Fruit& fruit) const
{
	float x = animal.getPosition().x - fruit.getPosition().x;
	float y = animal.getPosition().y - fruit.getPosition().y;

	float distance = sqrt(pow(x, 2) + pow(y, 2));

	return distance <= animal.getRadius() + fruit.getRadius();
}

void Ecosystem::eat(Animal& animal, Fruit& fruit)
{
	if (fruit.getEnergy() + animal.getHp() > animal.getMaxHp())
	{
		fruit.setEnergy(fruit.getEnergy() + animal.getHp() - animal.getMaxHp());
		animal.setHp(animal.getMaxHp());

		// clone using energy surplus!:
		while (fruit.getEnergy() > animal.getMaxHp())
		{
			fruit.setEnergy(fruit.getEnergy() - animal.getMaxHp());
			
			this->animals.push_back(std::make_shared<Animal>(animal));

			this->animals.back()->setHp(animal.getMaxHp());
			this->animals.back()->setVelocity(sf::Vector2f(0.f, 0.f));
			this->animals.back()->randomMutate(this->mutationPercentage);

			if (&animal == this->trackedAnimal)
				this->animals.back()->setColor(this->animalsColor);

			this->hpBarsVisibility[this->animals.back().get()] = this->hpBarsVisibility[&animal];
			this->brainsPreviewsVisibility[this->animals.back().get()] = this->brainsPreviewsVisibility[&animal];
		}

		this->animals.push_back(std::make_shared<Animal>(animal));

		this->animals.back()->setHp(fruit.getEnergy());
		this->animals.back()->setVelocity(sf::Vector2f(0.f, 0.f));
		this->animals.back()->randomMutate(this->mutationPercentage);

		if (&animal == this->trackedAnimal)
			this->animals.back()->setColor(this->animalsColor);

		this->hpBarsVisibility[this->animals.back().get()] = this->hpBarsVisibility[&animal];
		this->brainsPreviewsVisibility[this->animals.back().get()] = this->brainsPreviewsVisibility[&animal];
	}
	else
		animal.increaseHp(fruit.getEnergy());

	fruit.setEnergy(0.0f);
}

void Ecosystem::removeEatenFruits()
{
	for (int i = 0; i < this->fruits.size();)
	{
		if (this->fruits[i]->getEnergy() == 0.0f)
			this->removeFruit(this->fruits[i]);
		else
			i++;
	}
}

void Ecosystem::removeFruit(std::unique_ptr<Fruit>& fruit)
{
	std::swap(fruit, this->fruits.back());
	this->fruits.pop_back();
}

void Ecosystem::transferEnergyFromAnimalsToFruits()
{
	Fruit* lowestEnergyFruit = this->getLowestEnergyFruit();

	for (const auto& animal : this->animals)
		lowestEnergyFruit->increaseEnergy(animal->getEnergyToExpel());
}

Fruit* Ecosystem::getLowestEnergyFruit()
{
	if (this->fruits.empty())
		return nullptr;
	
	Fruit* lowestEnergyFruit = this->fruits[0].get();

	for (const auto& fruit : this->fruits)
		if (fruit->getEnergy() < lowestEnergyFruit->getEnergy())
			lowestEnergyFruit = fruit.get();

	return lowestEnergyFruit;
}

void Ecosystem::correctBrainPreviewsPositions()
{
	for (auto& animal : this->animals)
	{
		// put an interation into a methods?:
		const gui::NeuralNetPreview& brainPreview = animal->getBrainPreview();

		bool protrudesWorldRightBorder = this->brainPreviewProtrudesWorldRightBorder(brainPreview);
		bool protrudesWorldBottomBorder = this->brainPreviewProtrudesWorldBottomBorder(brainPreview);

		if (protrudesWorldRightBorder)
		{
			animal->setBrainPreviewPosition(
				animal->getPosition().x - animal->getBrainPreview().getSize().x,
				animal->getBrainPreview().getPosition().y
			);
		}
		else
		{
			animal->setBrainPreviewPosition(
				animal->getPosition().x,
				animal->getBrainPreview().getPosition().y
			);
		}

		if (protrudesWorldBottomBorder)
		{
			animal->setBrainPreviewPosition(
				animal->getBrainPreview().getPosition().x,
				animal->getPosition().y - animal->getBrainPreview().getSize().y
			);
		}
		else
		{
			animal->setBrainPreviewPosition(
				animal->getBrainPreview().getPosition().x,
				animal->getPosition().y
			);
		}
	}
}

bool Ecosystem::brainPreviewProtrudesWorldRightBorder(const gui::NeuralNetPreview& brain_preview)
{
	float rightBorderPosition = brain_preview.getPosition().x + brain_preview.getSize().x;

	return rightBorderPosition > this->getWorldSize().x;
}

bool Ecosystem::brainPreviewProtrudesWorldBottomBorder(const gui::NeuralNetPreview& brain_preview)
{
	float bottomBorderPosition = brain_preview.getPosition().y + brain_preview.getSize().y;

	return bottomBorderPosition > this->getWorldSize().y;
}
