#pragma once

#include "Animal.h"
#include "Food.h"
#include "EventsAccessor.h"

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

	// other public methods:
	void update(
		float dt, 
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mouse_pos_view, 
		bool paused,
		const std::string& god_tool
	);
	void render(sf::RenderTarget& target);

	// TODO: rmv later:
	bool shownDebugHps = false;

private:
	// private static variables:
	static std::string templateConfigFilePath;
	static std::string configFileName;
	static std::string foodFileName;

	// other private variables:
	std::string directoryPath;

	sf::Vector2f worldSize;

	unsigned borderThickness;

	sf::RectangleShape border;
	sf::RectangleShape background;

	std::vector<Animal*> animals;
	std::vector<Food*> food;

	Animal* trackedAnimal;

	// private utilities:
	std::vector<CrappyNeuralNets::Scalar> getInputsForBrain(const Animal& animal) const;

	Food* findTheNearestFood(const Animal& animal) const;

	void useGodTool(
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mouse_pos_view,
		const std::string& tool
	);

	void removeDeadAnimals();

	void avoidGoingBeyondTheWorld();

	void feedAnimalsWithFood();

	void feedWithNlogN();

	void feedWithSquare();

	static bool compareAnimalsYPositions(const Animal* a1, const Animal* a2);

	bool animalIsToHigh(const Animal& animal, const Food& food);

	bool animalReachesFoodInY(const Animal& animal, const Food& food);
	bool animalReachesFood(const Animal& animal, const Food& food);

	int tryToFindConsumer(Food& fruit, unsigned start_animal_index);

	void eat(Animal& animal, Food& fruit);

	void removeEatenFood();

	// God tools:
	void track(const sf::Vector2f& mouse_pos_view);
	
	void remove(const sf::Vector2f& mouse_pos_view);
	
	void replace(const sf::Vector2f& mouse_pos_view);
	
	void brainVisibility(const sf::Vector2f& mouse_pos_view);
	
	void clone(const sf::Vector2f& mouse_pos_view);
	
	void stop(const sf::Vector2f& mouse_pos_view);
};
