#pragma once

#include "Crappy Neural Nets/RandomNumbersGenerator.h"

class Fruit
{
public:
	// constructors:
	Fruit();
	Fruit(
		float energy,
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);

	// public methods:
	void saveToFile(const std::string& path) const;
	void loadFromFile(const std::string& path);

	void render(sf::RenderTarget& target) const;

	bool isCovered(const sf::Vector2f& mouse_pos_view) const;

	// mutators:
	void setEnergy(float energy);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& new_position);

	void setRandomPosition(const sf::Vector2f& world_size, float borders_thickness);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

	// accessors:
	float getEnergy() const;

	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	const sf::Color& getColor() const;

private:
	float energy;

	sf::CircleShape shape;

	// private methods:
	// initialization:
	void initShape(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
};
