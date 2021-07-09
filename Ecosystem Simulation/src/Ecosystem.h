#pragma once

#include "Animal.h"
#include "Fruit.h"
#include "EventsAccessor.h"
#include "ProgressBar.h"
#include "NeuralNetPreview.h"

class Ecosystem
{
public:	
	// public static methods:
	static void createConfigFile(const std::string& folder_path);
	static void setUpEcosystemFolder(const std::string& folder_path);
	
	// constructor/destructor:
	Ecosystem();
	~Ecosystem();

	// initialization:
	void loadFromFolder(const std::string& folder_path);

	// accessors:
	const sf::Vector2f& getWorldSize() const;

	const std::string& getDirectoryPath() const;

	unsigned getBorderThickness() const;

	void printAnimalsPositions() const;

	const sf::Vector2f* getTrackedAnimalPosition() const;

	float getTotalTimeElapsed() const;

	bool isInitialized() const;

	const Animal* getTrackedAnimal() const;

	float getSimulationSpeedFactor() const;

	const std::string& getGodTool() const;

	// mutators:
	void setSimulationSpeedFactor(float simulation_speed_factor);

	void pauseSimulation();
	void unpauseSimulation();

	void setGodTool(const std::string& god_tool);

	// other public methods:
	void update(
		float dt,
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mouse_pos_view
	);
	void render(sf::RenderTarget& target);

private:
	// private static variables:
	static std::string templateConfigFilePath;
	static std::string configFileName;
	static std::string fruitsFileName;

	// other private variables:
	std::string directoryPath;

	sf::Vector2f worldSize;

	unsigned borderThickness;

	sf::RectangleShape border;
	sf::RectangleShape background;

	unsigned animalsCount;
	unsigned fruitsCount;

	std::vector<Animal*> animals;
	std::vector<Fruit*> fruits;

	Animal* trackedAnimal;

	float totalTimeElapsed;
	float dtSinceLastWorldUpdate;

	bool initialized;

	float simulationSpeedFactor;

	bool m_simulationIsPaused;

	std::string godTool;

	// private initialization:
	void readDataFromConfigFile(const std::string& folder_path);
	void initBorders();
	void initBackground();
	void initAnimals(const std::string& folder_path);
	void initFruits(const std::string& folder_path);

	// private utilities:
	std::vector<CrappyNeuralNets::Scalar> getInputsForBrain(const Animal& animal) const;

	Fruit* findTheNearestFruit(const Animal& animal) const;

	void useGodTool(
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mouse_pos_view
	);

	void updateWorld(float dt);

	void removeDeadAnimals();

	void convertKineticEnergyToFruit(Animal* animal, bool random_pos);

	void removeAnimal(Animal*& animal);

	void avoidGoingBeyondTheWorld();

	void feedAnimals();

	static bool compareAnimalsYPositions(const Animal* a1, const Animal* a2);

	bool animalIsToHigh(const Animal& animal, const Fruit& Fruit);

	bool animalReachesFruitInY(const Animal& animal, const Fruit& Fruit);
	bool animalReachesFruit(const Animal& animal, const Fruit& Fruit);

	int tryToFindConsumer(Fruit& fruit, unsigned start_animal_index);

	void eat(Animal& animal, Fruit& fruit);

	void removeEatenFruit();
	
	void convertAnimalToFruit(Animal*& animal);

	// God tools:
	void trackingTool(const sf::Vector2f& mouse_pos_view);
	
	void killingTool(const sf::Vector2f& mouse_pos_view);
	
	void replacingTool(const sf::Vector2f& mouse_pos_view);
	
	void brainTool(const sf::Vector2f& mouse_pos_view);
	
	void stoppingTool(const sf::Vector2f& mouse_pos_view);

	void infoTool(const sf::Vector2f& mouse_pos_view);
};
