#pragma once

#include "Crappy Neural Nets/NeuralNet.h"

class Ecosystem
{
public:
	// constructor:
	Ecosystem(
		const sf::Vector2u& world_size = sf::Vector2u(3840U, 2160U),
		const std::string& name = "DEFAULT NAME",
		unsigned border_size = 32U
	);

	// accessors:
	const sf::Vector2u& getWorldSize() const;

	const std::string& getName() const;

	unsigned getBorderSize() const;

	// public methods:
	void update();
	void render(sf::RenderTarget& target);

private:
	sf::Vector2u worldSize;

	std::string name;

	unsigned borderSize;

	sf::RectangleShape border;
	sf::RectangleShape background;
};
