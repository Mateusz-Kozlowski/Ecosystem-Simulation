#include "stdafx.h"
#include "Food.h"

// constructor:
Food::Food(float x, float y)
{
	this->shape.setPosition(x, y);
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setPointCount(16.f);
	this->shape.setRadius(4.f);
}

// accessors:
const sf::Vector2f& Food::getPosition() const
{
	return this->shape.getPosition();
}

// other public methods:
void Food::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	std::cout << "RENDER FOOD\n";
}
