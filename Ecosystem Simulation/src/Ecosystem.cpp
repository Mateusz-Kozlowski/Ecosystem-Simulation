#include "stdafx.h"
#include "Ecosystem.h"

// static variables:
std::string Ecosystem::templateConfigFilePath = "resources/template files/ecosystem initializator.ini";
std::string Ecosystem::configFileName = "ecosystem initializator.ini";
std::string Ecosystem::foodFileName = "food.ini";

// public static methods:
void Ecosystem::createConfigFile(const std::string& folder_path)
{
	std::filesystem::create_directories(folder_path);

	std::ifstream src(Ecosystem::templateConfigFilePath, std::ios::binary);
	std::ofstream dst(folder_path + "/" + Ecosystem::configFileName, std::ios::binary);

	if (!src.is_open()) throw("ERROR::CANNOT OPEN A FILE: " + Ecosystem::templateConfigFilePath);
	if (!dst.is_open()) throw("ERROR::CANNOT OPEN A FILE: " + folder_path + '/' + Ecosystem::configFileName);

	dst << src.rdbuf(); // copy from template file

	dst.close();
}

void Ecosystem::setUpEcosystemFolder(const std::string& folder_path)
{
	// read data from a config file:
	std::ifstream file(folder_path + '/' + Ecosystem::configFileName);

	if (!file.is_open()) throw("ERROR::ECOSYSTEM::CANNOT OPEN A FILE: " + folder_path + '/' + Ecosystem::configFileName);

	std::string temp;
	sf::Vector2f worldSize;
	float borderThickness;
	unsigned animalsCount = 0U, foodCount = 0U;

	file >> temp >> worldSize.x;
	file >> temp >> worldSize.y;
	file >> temp >> borderThickness;
	file >> temp >> animalsCount;
	file >> temp >> foodCount;

	file.close();

	// create folders for animals:
	for (int i = 0; i < animalsCount; i++)
		Animal::setUpAnimalFolder(folder_path + '/' + "animal" + std::to_string(i));

	// food file:
	std::ofstream foodFile(folder_path + '/' + Ecosystem::foodFileName);

	if (!foodFile.is_open()) throw("ERROR::ECOSYSTEM::CANNOT OPEN A FILE: " + folder_path + '/' + Ecosystem::foodFileName);

	std::stringstream ss;
	
	CrappyNeuralNets::RandomNumbersGenerator generator;

	for (float i = 0.f; i < foodCount; i++) 
	{
		Food f(10e2);
		f.setRandomPos(worldSize, borderThickness, generator);
		ss << f.getPosition().x << ' ' << f.getPosition().y << '\n';
	}

	foodFile << ss.str();

	foodFile.close();
}

// constructor/destructor:
Ecosystem::Ecosystem()
	: directoryPath("ECOSYSTEM IS NOT INITIALIZED"), borderThickness(0U), trackedAnimal(nullptr),
	  totalTimeElapsed(0.f), dtSinceLastWorldUpdate(0.f)
{
	
}

Ecosystem::~Ecosystem()
{
	for (auto& animal : this->animals) delete animal;
	
	for (auto& food : this->food) delete food;
}

// initialization:
void Ecosystem::loadFromFolder(const std::string& folder_path)
{
	this->directoryPath = folder_path;
	
	// read data from config file:
	std::ifstream file(folder_path + '/' + Ecosystem::configFileName);

	if (!file.is_open()) std::cerr << "ERROR::ECOSYSTEM::CANNOT OPEN FILE: " + folder_path + '/' + Ecosystem::configFileName;

	std::string temp;
	unsigned animalsCount = 0U, foodCount = 0U;

	file >> temp >> this->worldSize.x;
	file >> temp >> this->worldSize.y;
	file >> temp >> this->borderThickness;
	file >> temp >> animalsCount;
	file >> temp >> foodCount;

	file.close();

	// borders and backgroundRect:
	this->border.setFillColor(sf::Color(48, 48, 48));
	this->border.setSize(this->worldSize);

	this->background.setFillColor(sf::Color(32, 32, 32));
	this->background.setSize(
		sf::Vector2f(
			this->worldSize.x - 2.f * this->borderThickness,
			this->worldSize.y - 2.f * this->borderThickness
		)
	);
	this->background.setPosition(this->borderThickness, this->borderThickness);

	// animals:
	for (int i = 0; i < animalsCount; i++)
	{	
		this->animals.push_back(new Animal());
		this->animals[i]->loadFromFolder(folder_path + '/' + "animal" + std::to_string(i));
	}

	// food:
	// TODO: add static food name file
	std::ifstream file1(folder_path + '/' + "food.ini");

	if (!file1.is_open()) std::cerr << "ERROR::ECOSYSTEM::CANNOT OPEN FILE: " + folder_path + '/' + Ecosystem::configFileName;

	this->food.reserve(foodCount);

	float x, y;

	for (int i = 0; i < foodCount; i++)
	{
		file1 >> x;
		file1 >> y;
		this->food.push_back(new Food(10e6));
		this->food.back()->setPosition(x, y);
	}
}

// accessors:
const sf::Vector2f& Ecosystem::getWorldSize() const
{
	return this->worldSize;
}

const std::string& Ecosystem::getDirectoryPath() const
{
	return this->directoryPath;
}

unsigned Ecosystem::getBorderThickness() const
{
	return this->borderThickness;
}

void Ecosystem::printAnimalsPositions() const
{
	std::cout << "Animal positions:\n";
	for (const auto& animal : this->animals)
		std::cout << animal->getPosition().x << ' ' << animal->getPosition().y << '\n';
}

const sf::Vector2f* Ecosystem::getTrackedAnimalPosition() const
{
	if (this->trackedAnimal) return &this->trackedAnimal->getPosition();
	
	return nullptr;
}

float Ecosystem::getTotalTimeElapsed() const
{
	return this->totalTimeElapsed;
}

// other public methods:
void Ecosystem::update(
	float dt,
	float speed_factor,
	const std::vector<sf::Event>& events,
	const sf::Vector2f& mouse_pos_view, 
	bool paused,
	const std::string& god_tool)
{
	this->totalTimeElapsed += dt;
	
	this->useGodTool(events, mouse_pos_view, god_tool);

	if (!paused) this->updateWorld(dt, speed_factor);
}

void Ecosystem::render(sf::RenderTarget& target)
{
	target.draw(this->border);
	target.draw(this->background);

	for (const auto& animal : this->animals) animal->renderBody(target);
	
	for (const auto& animal : this->animals) animal->renderHpBar(target);
	
	for (const auto& food : this->food) food->render(target);

	for (const auto& animal : this->animals)
		if (animal->isBrainRendered())
			animal->renderBrain(target);
}

// private methods:
// private utilities:
std::vector<CrappyNeuralNets::Scalar> Ecosystem::getInputsForBrain(const Animal& animal) const
{
	std::vector<CrappyNeuralNets::Scalar> inputsForBrain;
	
	inputsForBrain.reserve(5);

	inputsForBrain.push_back(animal.getVelocity().x);
	inputsForBrain.push_back(animal.getVelocity().y);
	
	inputsForBrain.push_back(log2(animal.getHp()));

	Food* theNearestFood = findTheNearestFood(animal);
	
	inputsForBrain.push_back(theNearestFood->getPosition().x - animal.getPosition().x);
	inputsForBrain.push_back(theNearestFood->getPosition().y - animal.getPosition().y);

	return inputsForBrain;
}

Food* Ecosystem::findTheNearestFood(const Animal& animal) const
{
	Food* theNearestFood = this->food[0];
	float theSmallestDistance = INFINITY;

	for (const auto& it : this->food)
	{
		float a = animal.getPosition().x - it->getPosition().x;
		float b = animal.getPosition().y - it->getPosition().y;

		float distance = sqrt(pow(a, 2) + pow(b, 2));

		if (distance < theSmallestDistance)
		{
			theSmallestDistance = distance;
			theNearestFood = it;
		}
	}
	
	return theNearestFood;
}

void Ecosystem::useGodTool(
	const std::vector<sf::Event>& events,
	const sf::Vector2f& mouse_pos_view, 
	const std::string& god_tool)
{
	// TODO: add info that if there is no God tool then god_tool string should be equal to ""

	if (!EventsAccessor::hasEventOccured(sf::Event::MouseButtonPressed, events))
		return;

	// now use God tool:
	if (god_tool == "TRACK") this->track(mouse_pos_view);

	else if (god_tool == "REMOVE") this->remove(mouse_pos_view);

	else if (god_tool == "REPLACE") this->replace(mouse_pos_view);

	else if (god_tool == "BRAIN") this->brainVisibility(mouse_pos_view);

	else if (god_tool == "CLONE") this->clone(mouse_pos_view);

	else if (god_tool == "STOP") this->stop(mouse_pos_view);

	else if (god_tool == "") return;

	else
	{
		std::cerr << "INCORRECT GOD TOOL: " << god_tool << '\n';
		exit(-1);
	}
}

void Ecosystem::updateWorld(float dt, float speed_factor)
{
	for (const auto& animal : this->animals)
		animal->updateBodyAndHp(dt, speed_factor, getInputsForBrain(*animal));

	this->removeDeadAnimals();

	this->avoidGoingBeyondTheWorld();

	this->feedAnimals();

	this->removeEatenFood();

	for (auto& animal : this->animals)
		if (animal->isBrainRendered())
			animal->updateBrainPreview();
}

void Ecosystem::removeDeadAnimals()
{
	for (int i = 0; i < this->animals.size(); i++)
		if (!this->animals[i]->isAlive())
		{
			delete this->animals[i];
			std::swap(this->animals[i], this->animals.back());
			this->animals.pop_back();
		}
}

void Ecosystem::avoidGoingBeyondTheWorld()
{
	// avoid going beyond the world:
	for (auto& animal : this->animals)
	{
		// left border:
		if (animal->getPosition().x - animal->getRadius() < this->borderThickness)
		{
			animal->setVelocity({ -animal->getVelocity().x, animal->getVelocity().y });
			animal->setPosition(sf::Vector2f(this->borderThickness + animal->getRadius(), animal->getPosition().y));
		}

		// right border:
		else if (animal->getPosition().x + animal->getRadius() > this->worldSize.x - this->borderThickness)
		{
			animal->setVelocity({ -animal->getVelocity().x, animal->getVelocity().y });
			animal->setPosition(sf::Vector2f(this->worldSize.x - this->borderThickness - animal->getRadius(), animal->getPosition().y));
		}

		// it is possible that an animal crossed 2 perpendicular borders in the same frame, so we don't use else if here:
		// top border:
		if (animal->getPosition().y - animal->getRadius() < this->borderThickness)
		{
			animal->setVelocity({ animal->getVelocity().x, -animal->getVelocity().y });
			animal->setPosition(sf::Vector2f(animal->getPosition().x, this->borderThickness + animal->getRadius()));
		}

		// bottom border:
		else if (animal->getPosition().y + animal->getRadius() > this->worldSize.y - this->borderThickness)
		{
			animal->setVelocity({ animal->getVelocity().x, -animal->getVelocity().y });
			animal->setPosition(sf::Vector2f(animal->getPosition().x, this->worldSize.y - this->borderThickness - animal->getRadius()));
		}
	}
}

void Ecosystem::feedAnimals()
{
	std::sort(this->animals.begin(), this->animals.end(), compareAnimalsYPositions);

	for (int f = 0; f < this->food.size(); f++)
	{
		unsigned left = 0, right = this->animals.size() - 1U;

		while (left < right)
		{
			unsigned center = (left + right) / 2;

			if (this->animalIsToHigh(*this->animals[center], *this->food[f]))
			{
				left = center + 1;
			}
			else
			{
				right = center;
			}
		}

		this->tryToFindConsumer(*this->food[f], left);
	}
}

bool Ecosystem::compareAnimalsYPositions(const Animal* a1, const Animal* a2)
{
	return a1->getPosition().y < a2->getPosition().y;
}

bool Ecosystem::animalIsToHigh(const Animal& animal, const Food& food)
{
	return (animal.getPosition().y - food.getPosition().y) < -(animal.getRadius() + food.getRadius());
}

bool Ecosystem::animalReachesFoodInY(const Animal& animal, const Food& food)
{
	if (abs(animal.getPosition().y - food.getPosition().y) <= animal.getRadius() + food.getRadius()) return true;
	
	return false;
}

bool Ecosystem::animalReachesFood(const Animal& animal, const Food& food)
{
	float a = animal.getPosition().x - food.getPosition().x;
	float b = animal.getPosition().y - food.getPosition().y;

	float distance = sqrt(pow(a, 2) + pow(b, 2));

	if (distance <= animal.getRadius() + food.getRadius()) return true;

	return false;
}

int Ecosystem::tryToFindConsumer(Food& fruit, unsigned start_animal_index)
{
	unsigned animal_index = start_animal_index;

	while (animal_index < this->animals.size() && this->animalReachesFoodInY(*this->animals[animal_index], fruit))
	{
		if (this->animalReachesFood(*this->animals[animal_index], fruit))
		{
			this->eat(*this->animals[animal_index], fruit);
			return 1;
		}
		animal_index++;
	}

	return 0;
}

void Ecosystem::eat(Animal& animal, Food& fruit)
{
	animal.increaseHp(fruit.getEnergy());
	fruit.setEnergy(0.f);
}

void Ecosystem::removeEatenFood()
{
	for (int i = 0; i < this->food.size(); i++)
		if (this->food[i]->getEnergy() == 0.f)
		{
			delete this->food[i];
			std::swap(this->food[i], this->food.back());
			this->food.pop_back();
		}
}

// God tools:
void Ecosystem::track(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
	{
		if (animal->isCovered(mouse_pos_view))
		{
			// the tracked animal is no longer tracked:
			if (animal == this->trackedAnimal)
			{
				animal->setColor(sf::Color::Red); // reset its color
				this->trackedAnimal = nullptr;
				return;
			}
			
			// a new animal is tracked:
			// reset body color of the previous tracked animal:
			if (this->trackedAnimal) this->trackedAnimal->setColor(sf::Color::Red);
			
			// set new tracked animal:
			this->trackedAnimal = animal;
			
			// change color:
			animal->setColor(sf::Color(150, 0, 200));

			return;
		}
	}
}

void Ecosystem::remove(const sf::Vector2f& mouse_pos_view)
{
	// animals:
	for (auto& animal : this->animals)
	{
		if (animal->isCovered(mouse_pos_view))
		{
			delete animal;
			std::swap(animal, this->animals.back());
			this->animals.pop_back();
			break;
		}
	}

	// food:
	for (auto& f : this->food)
	{
		if (f->isCovered(mouse_pos_view))
		{			
			delete f;
			std::swap(f, this->food.back());
			this->food.pop_back();
			return;
		}
	}
}

void Ecosystem::replace(const sf::Vector2f& mouse_pos_view)
{
}

void Ecosystem::brainVisibility(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
	{
		if (animal->isCovered(mouse_pos_view))
		{
			animal->setBrainIsRendered(!animal->isBrainRendered());
			return;
		}
	}
}

void Ecosystem::clone(const sf::Vector2f& mouse_pos_view)
{
}

void Ecosystem::stop(const sf::Vector2f& mouse_pos_view)
{
	for (auto& animal : this->animals)
	{
		if (animal->isCovered(mouse_pos_view))
		{
			animal->setVelocity(sf::Vector2f(0.f, 0.f));
			return;
		}
	}
}
