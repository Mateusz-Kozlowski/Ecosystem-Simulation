#pragma once

#include "Animal.h"
#include "Food.h"

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

	// other public methods:
	void update(float dt, const std::vector<sf::Event>& events, const sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);

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

	// private utilities:
	std::vector<CrappyNeuralNets::Scalar> getInputsForBrain(const Animal& animal);

	Food* findTheNearestFood(const Animal& animal);
};
