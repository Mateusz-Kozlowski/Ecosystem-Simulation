#pragma once

#include "Crappy Neural Nets/RandomNumbersGenerator.h"

class Fruit
{
public:
	// constructor:
	Fruit(float energy);

	// mutators:
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& new_position);

	void setRandomPos(const sf::Vector2f& worldSize, float bordersThickness);
	void setEnergy(float energy);

	// accessors:
	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	float getEnergy() const;

	// other public methods:
	void render(sf::RenderTarget& target) const;

	bool isCovered(const sf::Vector2f& mouse_pos_view) const;

private:
	sf::CircleShape shape;

	float energy;
};
