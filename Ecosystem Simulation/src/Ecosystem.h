#pragma once

#include "Animal.h"
#include "Fruit.h"
#include "EventsAccessor.h"
#include "GodTools.h"
#include "SideMenu.h"

class Ecosystem
{
public:
	Ecosystem(
		const std::string& name,
		const sf::Vector2f& worldSize,
		float bordersThickness,
		const sf::Color& backgroundColor,
		const sf::Color& bordersColor,
		unsigned animalsCount,
		unsigned fruitsCount,
		float animalsRadius,
		float fruitsRadius,
		double defaultAnimalsHp,
		double defaultFruitsEnergy,
		float mutationPercentage,
		const sf::Color& animalsColor,
		const sf::Color& fruitsColor,
		const sf::Color& trackedAnimalColor,
		float simulationSpeedFactor = 1.0f,
		bool simulationIsPaused = false,
		GodTool godTool = GodTool::NONE,
		bool renderHpBarsByDefault = true,
		bool renderBrainsByDefault = true
	);
	Ecosystem(const char* folderPath);
	
	void saveToFolder(const std::string& folderPath) const;
	void loadFromFolder(const std::string& folderPath);

	void useGodTools(
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mousePosView
	);

	void update(float dt);
	void render(sf::RenderTarget& target) const;

	// accessors:
	
	const std::string& getName() const;

	float calcFruitsDensity() const;
	float calcArenaArea() const;
	const sf::Vector2f& getArenaSize() const;
	sf::Vector2f getWorldSize() const;

	float getBordersThickness() const;
	unsigned getAnimalsCount() const;

	const std::vector<std::shared_ptr<Animal>>& getAnimals() const;

	unsigned getFruitsCount() const;

	float getMutationPercentage() const;

	const sf::Color& getBackgroundColor() const;
	const sf::Color& getBordersColor() const;
	const sf::Color& getAnimalsColor() const;
	const sf::Color& getFruitsColor() const;
	const sf::Color& getTrackedAnimalColor() const;

	const Animal* getTrackedAnimal() const;

	float getSimulationSpeedFactor() const;

	bool isSimulationPaused() const;

	GodTool getCurrentGodTool() const;

	const std::unordered_map<Animal*, bool>& getBrainsVisibility() const;

	float getTotalTimeElapsed() const;

	double getTotalAnimalsHpEnergy() const;
	double getTotalAnimalsKineticEnergy() const;
	double getTotalFruitsEnergy() const;
	double getTotalEnergy() const;

	void printAllAnimalsPositions() const;

	// mutators:
	
	void setName(const std::string& name);

	void setMutationPercentage(float mutationPercentage);

	void setBackgroundColor(const sf::Color& backgroundColor);

	void setBordersColor(const sf::Color& bordersColor);
	void setAnimalsColor(const sf::Color& animalsColor);
	void setFruitsColor(const sf::Color& fruitsColor);
	void setTrackedAnimalColor(const sf::Color& trackedAnimalColor);

	void setSimulationSpeedFactor(float simulationSpeedFactor);

	void pauseSimulation();
	void unpauseSimulation();

	void setGodTool(GodTool godTool);

	void hideAllBrainsPreviews();
	void showAllBrainsPreviews();

private:
	void initBackgroundAndBorders(
		const sf::Vector2f& worldSize,
		float bordersThickness,
		const sf::Color& backgroundColor,
		const sf::Color& bordersColor
	);

	void createNewAnimals(
		unsigned animalsCount,
		double defaultAnimalsHp,
		float animalsRadius,
		const sf::Color& animalsColor,
		bool renderHpBarsByDefault,
		bool renderBrainsByDefault
	);
	void createNewAnimal(
		double defaultAnimalHp,
		float animalRadius,
		const sf::Color& animalColor,
		bool renderHpBarByDefault,
		bool renderBrainByDefault
	);

	void createNewFruits(
		unsigned fruitsCount,
		double defaultFruitsEnergy,
		float fruitsRadius,
		const sf::Color& fruitsColor
	);
	void createNewFruit(
		double energy, 
		float radius, 
		const sf::Color& fruitColor
	);
	
	void saveAnimals(const std::string& folderPath) const;
	void saveFruits(const std::string& folderPath) const;
	void saveEcosystem(const std::string& filePath) const;
	void saveHpBarsVisibility(const std::string& filePath) const;
	void saveBrainsPreviewsVisibility(const std::string& filePath) const;

	void loadAnimals(const std::string& folderPath);
	void loadAnimal(const std::string& folderPath);

	void loadFruits(const std::string& folderPath);
	void loadFruit(const std::string& filePath);

	void loadEcosystem(const std::string& filePath);
	void makeTracked(Animal& animal);

	void loadHpBarsVisibility(const std::string& filePath);
	
	void loadBrainsPreviewsVisibility(const std::string& filePath);

	int getTrackedAnimalIndex() const;

	void mutatingTool(const sf::Vector2f& mousePosView);

	void trackingTool(const sf::Vector2f& mousePosView);
	void stopTracking();

	void killingTool(const sf::Vector2f& mousePosView);
	void convertAnimalToFruit(
		std::shared_ptr<Animal>& animal, 
		bool randomFruitPosition
	);

	void replacingTool(const sf::Vector2f& mousePosView);

	void brainTool(const sf::Vector2f& mousePosView);

	void stoppingTool(const sf::Vector2f& mousePosView);
	void convertKineticEnergyToFruit(Animal& animal, bool randomFruitPosition);

	void infoTool(const sf::Vector2f& mousePosView) const;
	void printInfoAboutAnimal(const Animal& animal) const;
	void printInfoAboutFruit(const Fruit& fruit) const;
	void printInfoAboutEcosystem() const;
	
	void updateWorld(float dt);

	void updateAnimals(float dt);

	const std::vector<Blueberry::Scalar> getInputsForBrain(
		const Animal& animal
	) const;
	
	const Fruit* getTheNearestFruit(const Animal& animal) const;
	float calcDistance(const Animal& animal, const Fruit& fruit) const;

	void avoidTunneling();
	void avoidTunnelingByVerticalBorders(Animal& animal);
	void avoidTunnelingByHorizontalBorders(Animal& animal);

	void killAnimalsStickingToBorders();
	bool sticksToBorder(const Animal& animal);
	bool sticksToLeftBorder(const Animal& animal);
	bool sticksToRightBorder(const Animal& animal);
	bool sticksToTopBorder(const Animal& animal);
	bool sticksToBottomBorder(const Animal& animal);

	void removeDeadAnimals();
	void removeAnimal(std::shared_ptr<Animal>& animal);

	void feedAnimals(float dt);
	static bool compareAnimalsYPositions(
		std::shared_ptr<Animal> a1, 
		std::shared_ptr<Animal> a2
	);
	bool animalIsTooHigh(const Animal& animal, const Fruit& fruit) const;
	int tryToFindConsumer(
		Fruit& fruit, 
		unsigned startAnimalindex,
		float dt
	);
	bool animalReachesFruitInY(const Animal& animal, const Fruit& fruit) const;
	bool animalReachesFruit(const Animal& animal, const Fruit& fruit) const;
	void eat(Animal& animal, Fruit& fruit, float dt);
	
	void removeEatenFruits();
	void removeFruit(std::unique_ptr<Fruit>& fruit);

	void transferEnergyFromAnimalsToFruits();
	Fruit* getLowestEnergyFruit();

	void correctPopulationSize(float dt);
	void randomShuffleAnimals();
	static bool correctPopulationSizeComparator(
		std::shared_ptr<Animal> a1,
		std::shared_ptr<Animal> a2
	);

	void correctBrainPreviewsPositions();
	bool brainPreviewProtrudesWorldRightBorder(
		const gui::BrainPreview& brainPreview
	);
	bool brainPreviewProtrudesWorldBottomBorder(
		const gui::BrainPreview& brainPreview
	);

private:
	std::string m_name;

	sf::RectangleShape m_background;

	// m_animals vector contains shared pointers 
	// (in contrast to fruits vector which contains unique pointers)
	// to make cloning m_animals possible
	// (cloning occurs by calling a copy constructor of Animal class)
	std::vector<std::shared_ptr<Animal>> m_animals;
	std::vector<std::unique_ptr<Fruit>> m_fruits;

	// TODO: add a multiline comment that:
	// - explains why those variables are useful in those situations
	// - describes those situation (when they appear and how they disappear)

	// useful when there are temporarily no m_animals in an ecosystem:
	float m_animalsRadius;

	// useful when there are temporarily no fruits in an ecosystem:
	float m_fruitsRadius;

	double m_defaultAnimalsHp;

	float m_mutationPercentage;

	// useful when there are temporarily no m_animals in an ecosystem:
	sf::Color m_animalsColor;

	// useful when there are temporarily no fruits in an ecosystem:
	sf::Color m_fruitsColor;

	// useful when there is temporarily no tracked animal in an ecosystem
	sf::Color m_trackedAnimalColor;

	Animal* m_trackedAnimal;

	float m_simulationSpeedFactor;

	bool m_simulationIsPaused;

	GodTool m_godTool;

	std::unordered_map<Animal*, bool> m_hpBarsVisibility;
	std::unordered_map<Animal*, bool> m_brainsPreviewsVisibility;

	float m_totalTimeElapsed;
};
