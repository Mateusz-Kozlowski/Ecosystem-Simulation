#pragma once

#include "Libraries.h"
#include "RandomEngine.h"

class Fruit
{
public:
	Fruit(
		unsigned energy,
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	Fruit(const char* filePath);
	
	void saveToFile(const char* filePath) const;
	void loadFromFile(const char* filePath);

	void render(sf::RenderTarget& target) const;

	bool isCoveredByMouse(const sf::Vector2f& mousePosView) const;

	std::string toStr() const;

	// accessors:

	unsigned getEnergy() const;

	const sf::Vector2f& getPos() const;

	float getRadius() const;

	const sf::Color& getColor() const;

	// mutators:

	void setEnergy(unsigned energy);
	
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& newPosition);

	void setRandomPosition(
		const sf::Vector2f& arenaSize, 
		float bordersThickness,
		float marginsThickness,
		bool linearDistributionOfPositionProbability
	);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

private:
	void initShape(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);

	void setRandomLinearPositionUtil(
		const std::pair<float, float>& rangeX,
		const std::pair<float, float>& rangeY
	);
	void setRandomNonLinearPositionUtil(
		const std::pair<float, float>& rangeX,
		const std::pair<float, float>& rangeY
	);

	static bool belongsToArena(
		double r, 
		double alfa, 
		double arenaWidth, 
		double arenaHeight
	);
	
private:
	unsigned m_energy;

	sf::CircleShape m_shape;
};
