#pragma once

#include "Libraries.h"

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
		const sf::Vector2f& worldSize, 
		float bordersThickness,
		float marginsSize
	);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

private:
	void initShape(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	
private:
	unsigned m_energy;

	sf::CircleShape m_shape;
};
