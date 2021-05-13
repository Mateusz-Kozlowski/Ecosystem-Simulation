#pragma once

#include "Crappy Neural Nets/RandomNumbersGenerator.h"

class Food
{
public:
	// constructor:
	Food();

	// mutators:
	void setPos(float x, float y);
	void setRandomPos(
		const sf::Vector2f& worldSize,
		float bordersThickness,
		CrappyNeuralNets::RandomNumbersGenerator& generator
	);

	// accessors:
	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	// other public methods:
	void render(sf::RenderTarget& target) const;

private:
	sf::CircleShape shape;
};
