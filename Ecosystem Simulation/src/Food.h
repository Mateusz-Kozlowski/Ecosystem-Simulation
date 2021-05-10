#pragma once

#include "Crappy Neural Nets/RandomNumbersGenerator.h"

class Food
{
public:
	// constructor:
	Food();

	// mutators:
	void setPos(float x, float y);
	void setRandomPos(const sf::Vector2f& worldSize, CrappyNeuralNets::RandomNumbersGenerator& generator);

	// accessors:
	const sf::Vector2f& getPosition() const;

	// other public methods:
	void render(sf::RenderTarget& target);

private:
	sf::CircleShape shape;
};
