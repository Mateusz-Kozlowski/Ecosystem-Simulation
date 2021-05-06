#pragma once

#include "Crappy Neural Nets/NeuralNet.h"

class Ecosystem
{
public:
	Ecosystem();

	std::string name;

	void update() {}
	void render(sf::RenderTarget& target);

private:
	std::vector<sf::CircleShape> c;

};
