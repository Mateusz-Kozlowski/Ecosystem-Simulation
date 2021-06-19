#pragma once

#include "Crappy Neural Nets/RandomNumbersGenerator.h"

class Food
{
public:
	// constructor:
	Food(float energy);

	// mutators:
	void setPosition(float x, float y);

	void setRandomPos(
		const sf::Vector2f& worldSize,
		float bordersThickness,
		CrappyNeuralNets::RandomNumbersGenerator& generator
	);
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
