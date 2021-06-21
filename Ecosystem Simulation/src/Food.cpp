#include "stdafx.h"
#include "Food.h"

// constructor:
Food::Food(float energy)
	: energy(energy)
{
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setPointCount(16.f);
	this->shape.setRadius(4.f);
	this->shape.setOrigin(this->shape.getRadius(), this->shape.getRadius());
}

// mutators:
void Food::setPosition(float x, float y)
{
	this->shape.setPosition(x, y);
}

void Food::setPosition(const sf::Vector2f& new_position)
{
	this->shape.setPosition(new_position);
}

void Food::setRandomPos(const sf::Vector2f& worldSize, float bordersThickness)
{
	std::pair<unsigned, unsigned> px = { bordersThickness, worldSize.x - bordersThickness };
	std::pair<unsigned, unsigned> py = { bordersThickness, worldSize.y - bordersThickness };

	float x = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(px);
	float y = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(py);

	this->shape.setPosition(x, y);
}

void Food::setEnergy(float energy)
{
	this->energy = energy;
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

float Food::getEnergy() const
{
	return this->energy;
}

// other public methods:
void Food::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
}

bool Food::isCovered(const sf::Vector2f& mouse_pos_view) const
{
	float acceleration = this->shape.getPosition().x - mouse_pos_view.x;
	float b = this->shape.getPosition().y - mouse_pos_view.y;
	
	float distance = sqrt(pow(acceleration, 2) + pow(b, 2));
	
	return this->getRadius() >= distance;
}
