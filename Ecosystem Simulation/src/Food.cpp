#include "stdafx.h"
#include "Food.h"

// constructor:
Food::Food()
{
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setPointCount(3.f);
	this->shape.setRadius(4.f);
}

// mutators:
void Food::setPos(float x, float y)
{
	this->shape.setPosition(x, y);
}

void Food::setRandomPos(const sf::Vector2f& worldSize, CrappyNeuralNets::RandomNumbersGenerator& generator)
{
	std::pair<unsigned, unsigned> px = { 0U, worldSize.x };
	std::pair<unsigned, unsigned> py = { 0U, worldSize.y };

	float x = generator.getRandomNumber(px);
	float y = generator.getRandomNumber(py);

	this->shape.setPosition(x, y);
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
}
