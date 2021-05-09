#pragma once

#include "Crappy Neural Nets/NeuralNet.h"
#include "Individual.h"

class Ecosystem
{
public:
	// constructor/destructor:
	Ecosystem();
	~Ecosystem();

	// accessors:
	const sf::Vector2f& getWorldSize() const;

	const std::string& getDirectoryPath() const;

	unsigned getBorderThickness() const;

	// public initialization methods:
	void create(const std::string& folder_path);
	void loadFromFolder(const std::string& folder_path);

	// other public methods:
	void update(float dt);
	void render(sf::RenderTarget& target);

	// static methods:
	static void createConfigFile(const std::string& directory_path);

	static std::string configFileName;

private:
	static std::string configFileTemplatePath;

	std::string directoryPath;

	sf::Vector2f worldSize;

	unsigned borderThickness;

	sf::RectangleShape border;
	sf::RectangleShape background;

	std::vector<Individual*> individuals;
};
