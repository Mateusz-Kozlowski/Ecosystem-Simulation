#pragma once

#include "Crappy Neural Nets/RandomNumbersGenerator.h"

class Fruit
{
public:
	Fruit(
		float energy,
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	Fruit(const std::string& file_path);

	// public methods:
	void saveToFile(const std::string& file_path) const;
	void loadFromFile(const std::string& file_path);

	void render(sf::RenderTarget& target) const;

	bool isCoveredByMouse(const sf::Vector2f& mouse_pos_view) const;

	// accessors:
	float getEnergy() const;

	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	const sf::Color& getColor() const;

	// mutators:
	void setEnergy(float energy);
	void increaseEnergy(float increase);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& new_position);

	void setRandomPosition(const sf::Vector2f& world_size, float borders_thickness);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

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
