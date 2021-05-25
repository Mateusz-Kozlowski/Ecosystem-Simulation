#include "stdafx.h"
#include "Food.h"

// constructor:
Food::Food()
{
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setPointCount(16.f);
	this->shape.setRadius(4.f);
	this->shape.setOrigin(this->shape.getRadius(), this->shape.getRadius());
}

// mutators:
void Food::setPos(float x, float y)
{
	this->shape.setPosition(x, y);
}

void Food::setRandomPos(
	const sf::Vector2f& worldSize, 
	float bordersThickness,
	CrappyNeuralNets::RandomNumbersGenerator& generator)
{
	std::pair<unsigned, unsigned> px = { bordersThickness, worldSize.x - bordersThickness };
	std::pair<unsigned, unsigned> py = { bordersThickness, worldSize.y - bordersThickness };

	float x = generator.getRandomNumber(px);
	float y = generator.getRandomNumber(py);

	this->shape.setPosition(x, y);
}

// accessors:
const sf::Vector2f& Food::getPosition() const
{
	return this->shape.getPosition();
}

float Food::getRadius() const
{
	return this->shape.getRadius();
}

// other public methods:
void Food::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
}

bool Food::isCovered(const sf::Vector2f& mouse_pos_view) const
{
	float a = this->shape.getPosition().x - mouse_pos_view.x;
	float b = this->shape.getPosition().y - mouse_pos_view.y;
	
	float distance = sqrt(pow(a, 2) + pow(b, 2));
	
	return this->getRadius() >= distance;
}
