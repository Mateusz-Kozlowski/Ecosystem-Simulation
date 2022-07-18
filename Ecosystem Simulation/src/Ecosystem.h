#pragma once

#include "Animal.h"
#include "Fruit.h"
#include "EventsAccessor.h"
#include "GodTools.h"
#include "SideMenu.h"

class Ecosystem
{
public:
	Ecosystem();
	Ecosystem(
		const std::string& name,
		const sf::Vector2f& worldSize,
		float bordersThickness,
		const sf::Color& backgroundColor,
		const sf::Color& bordersColor,
		unsigned initialAnimalsCount,
		unsigned fruitsCount,
		float animalsRadius,
		float fruitsRadius,
		float fruitsPositionsMarginsThickness,
		unsigned defaultAnimalsHp,
		unsigned defaultFruitsEnergy,
		unsigned mutationsPerMutation,
		float basalMetabolicRatePerFrame,
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
	~Ecosystem();
	
	void saveToFolder(const std::string& folderPath) const;
	void loadFromFolder(const std::string& folderPath);

	void useGodTools(
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mousePos
	);

	void update(
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events,
		const std::unordered_map<std::string, int>& keybinds
	);
	void render(sf::RenderTarget& target) const;

	// accessors:
	
	const std::string& getName() const;

	float calcFruitsDensity() const;
	float calcArenaArea() const;

	/*
	Returns size of the arena in which animals are able to move
	Do not confuse the func with Ecosystem::getWorldSize() const;
	*/
	const sf::Vector2f& getArenaSize() const;
	
	/*
	Returns the size of the entire world - arena and borders.
	Do not confuse the func with Ecosystem::getArenaSize() const;
	*/
	sf::Vector2f getWorldSize() const;

	float getBordersThickness() const;
	unsigned getAnimalsCount() const;

	const std::vector<std::shared_ptr<Animal>>& getAnimals() const;
	const std::vector<std::unique_ptr<Fruit>>& getFruits() const;

	unsigned getFruitsCount() const;

	unsigned getMutationsPerMutation() const;

	const sf::Color& getBackgroundColor() const;
	const sf::Color& getBordersColor() const;
	const sf::Color& getAnimalsColor() const;
	const sf::Color& getFruitsColor() const;
	const sf::Color& getTrackedAnimalColor() const;

	const Animal* getTrackedAnimal() const;
	const Animal* getAnimalWithModifiedBrain() const;

	float getSimulationSpeedFactor() const;

	bool isSimulationPaused() const;

	GodTool getCurrentGodTool() const;

	const std::unordered_map<Animal*, bool>& getBrainsVisibility() const;

	float getTotalTimeElapsed() const;

	unsigned getTotalAnimalsHpEnergy() const;
	unsigned getTotalAnimalsKineticEnergy() const;
	unsigned getTotalAnimalsPreviousKineticEnergy() const;
	unsigned getTotalFruitsEnergy() const;
	unsigned getTotalEnergy() const;

	void printAllAnimalsPositions() const;

	void printInfo() const;

	unsigned getTotalFramesElapsed() const;

	// mutators:
	
	void setName(const std::string& name);

	void setMutationsCount(float mutationsCount);

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

	void stopModifyingBrainPreview();

private:
	// initialization:

	void initBackgroundAndBorders(
		const sf::Vector2f& worldSize,
		float bordersThickness,
		const sf::Color& backgroundColor,
		const sf::Color& bordersColor
	);

	/*
	This func do not preserve energy. 
	If you want energy to be preserved, the new animals hp has to come from somewhere.
	*/
	void createNewAnimals(
		unsigned initialAnimalsCount,
		unsigned defaultAnimalsHp,
		float basalMetabolicRatePerFrame,
		float animalsRadius,
		const sf::Color& animalsColor,
		bool renderHpBarsByDefault,
		bool renderBrainsByDefault
	);

	/*
	This func do not preserve energy.
	If you want energy to be preserved, the new animal hp has to come from somewhere.
	*/
	void createNewAnimal(
		unsigned defaultAnimalHp,
		float basalMetabolicRatePerFrame,
		float animalRadius,
		const sf::Color& animalColor,
		bool renderHpBarByDefault,
		bool renderBrainByDefault
	);

	/*
	This func do not preserve energy.
	If you want energy to be preserved, the new fruits energy has to come from somewhere.
	*/
	void createNewFruits(
		unsigned fruitsCount,
		unsigned defaultFruitsEnergy,
		float fruitsRadius,
		const sf::Color& fruitsColor
	);

	/*
	This func do not preserve energy.
	If you want energy to be preserved, the new fruit energy has to come from somewhere.
	*/
	void createNewFruit(
		unsigned energy,
		float radius, 
		const sf::Color& fruitColor,
		bool linearDistributionOfPositionProbability
	);
	void initDebugFile();
	
	// utils:

	void saveAnimals(const std::string& folderPath) const;
	void saveFruits(const std::string& folderPath) const;
	void saveEcosystem(const std::string& filePath) const;
	
	/*
	Returns -1 if no animal is being tracked
	*/
	int getTrackedAnimalIndex() const;
	
	void saveHpBarsVisibility(const std::string& filePath) const;
	void saveBrainsPreviewsVisibility(const std::string& filePath) const;

	/*
	This func do not preserve energy
	*/
	void loadAnimals(const std::string& folderPath);

	/*
	This func do not preserve energy
	*/
	void loadAnimal(const std::string& folderPath);

	/*
	This func do not preserve energy
	*/
	void loadFruits(const std::string& folderPath);

	/*
	This func do not preserve energy
	*/
	void loadFruit(const std::string& filePath);

	/*
	This func do not preserve energy
	*/
	void loadEcosystem(const std::string& filePath);
	void makeTracked(Animal& animal);

	void loadHpBarsVisibility(const std::string& filePath);
	
	void loadBrainsPreviewsVisibility(const std::string& filePath);

	// God tools:

	// mutating tool:

	void mutatingTool(
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);

	// tracking tool:

	void trackingTool(const sf::Vector2f& mousePos);
	void stopTracking();

	// killing tool:

	/*
	Preserve energy by converting the killed animal energy to fruits energy
	using Ecosystem::convertAnimalToFruitsEnergy(std::shared_ptr<Animal>& animal) func
	*/
	void killingTool(const sf::Vector2f& mousePos);

	/*
	Safely converts animal to fruits energy 
	(increases energy of the fruit with the lowest energy)
	and preserves energy
	*/
	void convertAnimalToFruitsEnergy(std::shared_ptr<Animal>& animal);

	/*
	The function doesn't preserve energy; 
	in order to conserve it, the energy has to come from somewhere
	for instance that increase can be equal to energy expeled by animals
	*/
	void increaseFruitsEnergy(unsigned energyIncrease);

	/*
	Safely converts animal to a new fruit and conserves energy
	*/
	void convertAnimalToFruit(
		std::shared_ptr<Animal>& animal, 
		bool randomFruitPosition
	);

	// replacing tool:

	void replacingTool(const sf::Vector2f& mousePos);

	// brain tool:

	void brainTool(const sf::Vector2f& mousePos);

	// stopping tool:

	/*
	This func preserve energy
	*/
	void stoppingTool(const sf::Vector2f& mousePos);
	
	/*
	This func preserve energy
	*/
	void convertKineticEnergyToFruitsEnergy(Animal& animal);
	
	/*
	This func preserve energy
	*/
	void convertKineticEnergyToFruit(Animal& animal, bool randomFruitPosition);

	// info tool:

	void infoTool(const sf::Vector2f& mousePos) const;
	void printInfoAboutEcosystem() const;
	
	unsigned getClonesCount() const;

	void updateWorld(
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events,
		const std::unordered_map<std::string, int>& keybinds
	);

	void updateAnimals(
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events,
		const std::unordered_map<std::string, int>& keybinds
	);

	std::vector<Blueberry::Scalar> getEcosystemRelatedInputsForBrain(
		const Animal& animal
	) const;
	
	const Fruit* getTheNearestFruit(const Animal& animal) const;
	float calcDistance(const Animal& animal, const Fruit& fruit) const;

	/*
	Sometimes animals expel energy, which need to be handled in some way,
	if you want it to be conserved.
	This func increases fruits energy in order to make energy preserved.
	*/
	void expelEnergyFromAnimalsToFruits();
	Fruit* getLowestEnergyFruit();

	/*
	This func conserves energy
	*/
	void cloneAnimals(
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);
	bool isCloseToBorders(const Animal& animal);

	/*
	This func conserves energy
	*/
	void cloneAnimal(
		Animal& clonedAnimal,
		int idxOfClonedAnimal,
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);

	void avoidTunneling();
	void avoidTunnelingByVerticalBorders(Animal& animal);
	void avoidTunnelingByHorizontalBorders(Animal& animal);

	void kickInAssAnimalsStuckedNextToBorders();
	bool sticksToBorder(const Animal& animal);
	bool sticksToLeftBorder(const Animal& animal);
	bool sticksToRightBorder(const Animal& animal);
	bool sticksToTopBorder(const Animal& animal);
	bool sticksToBottomBorder(const Animal& animal);

	/*
	This func conserves energy
	*/
	void removeDeadAnimals();

	/*
	Safely removes an animal from the ecosystem APART FROM conserving energy;
	energy can be conserved by transfering it to fruits;
	transfering the total energy of a specific animal to fruits 
	can be achieved for example like this:
	increaseFruitsEnergy(animal.getTotalEnergy())
	*/
	void removeAnimal(std::shared_ptr<Animal>& animal);

	/*
	This func conserves energy
	*/
	void feedAnimals(
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);
	static bool compareAnimalsYPositions(
		std::shared_ptr<Animal> a1, 
		std::shared_ptr<Animal> a2
	);
	bool animalIsTooHigh(const Animal& animal, const Fruit& fruit) const;

	/*
	This func conserves energy
	*/
	int tryToFindConsumer(
		Fruit& fruit, 
		unsigned startAnimalindex,
		float dt,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);
	bool animalReachesFruitInY(const Animal& animal, const Fruit& fruit) const;
	bool animalReachesFruit(const Animal& animal, const Fruit& fruit) const;

	/*
	This func conserves energy
	*/
	void eat(
		Animal& animal, 
		Fruit& fruit
	);
	
	void relocateEatenFruits();

	/*
	This func conserves energy
	*/
	void redistributeFruitsEnergy(const std::unordered_map<std::string, int>& keybinds);
	static bool compareFruitsEnergy(
		std::unique_ptr<Fruit> f1,
		std::unique_ptr<Fruit> f2
	);
	
	void setHpBarsRanges();
	unsigned getTheBiggestHp() const;

	/*
	This func conserves energy
	*/
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

	void correctFruitsCount();
	
	void constValuesGuards();
	void totalEnergyGuard();
	void fruitsCountGuard();

	void debugLogs();

	void updateOnlyImgBtnsOfBrainsPreviews(
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);

	void updateModifyingBrainsPreviews();

	static std::string s_emptyEcosystemName;

private:
	std::string m_name;

	sf::RectangleShape m_bg;

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

	// TODO: no longer revelant, because there always will be some fruits:
	// useful when there are temporarily no fruits in an ecosystem:
	float m_fruitsRadius;
	float m_fruitsPositionsMarginsThickness;

	unsigned m_defaultAnimalsHp;
	unsigned m_defaultFruitEnergy;

	unsigned m_mutationsPerMutation;

	// useful when there are temporarily no m_animals in an ecosystem:
	sf::Color m_animalsColor;

	// useful when there are temporarily no fruits in an ecosystem:
	sf::Color m_fruitsColor;

	// useful when there is temporarily no tracked animal in an ecosystem
	sf::Color m_trackedAnimalColor;

	Animal* m_trackedAnimal;
	Animal* m_animalWithModifiedBrain;

	float m_simulationSpeedFactor;

	bool m_simulationIsPaused;

	GodTool m_godTool;

	std::unordered_map<Animal*, bool> m_hpBarsVisibility;
	std::unordered_map<Animal*, bool> m_brainsVisibility;

	float m_totalTimeElapsed;
	unsigned m_totalFramesElapsed;

	std::string m_logs;

	std::ofstream m_debugFile;

	unsigned m_previousTotalEnergy;
	unsigned m_fruitsCountGuard;
};
