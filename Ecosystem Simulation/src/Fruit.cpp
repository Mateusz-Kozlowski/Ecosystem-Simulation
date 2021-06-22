#include "stdafx.h"
#include "Fruit.h"

// constructor:
Fruit::Fruit(float energy)
	: energy(energy)
{
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setPointCount(16.f);
	this->shape.setRadius(4.f);
	this->shape.setOrigin(this->shape.getRadius(), this->shape.getRadius());
}

// mutators:
void Fruit::setPosition(float x, float y)
{
	this->shape.setPosition(x, y);
}

void Fruit::setPosition(const sf::Vector2f& new_position)
{
	this->shape.setPosition(new_position);
}

void Fruit::setRandomPos(const sf::Vector2f& worldSize, float bordersThickness)
{
	std::pair<unsigned, unsigned> px = { bordersThickness, worldSize.x - bordersThickness };
	std::pair<unsigned, unsigned> py = { bordersThickness, worldSize.y - bordersThickness };

	float x = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(px);
	float y = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(py);

	this->shape.setPosition(x, y);
}

void Fruit::setEnergy(float energy)
{
	this->energy = energy;
}

// accessors:
const sf::Vector2f& Fruit::getPosition() const
{
	return this->shape.getPosition();
}

float Fruit::getRadius() const
{
	return this->shape.getRadius();
}

float Fruit::getEnergy() const
{
	return this->energy;
}

// other public methods:
void Fruit::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
}

bool Fruit::isCovered(const sf::Vector2f& mouse_pos_view) const
{
	float acceleration = this->shape.getPosition().x - mouse_pos_view.x;
	float b = this->shape.getPosition().y - mouse_pos_view.y;
	
	float distance = sqrt(pow(acceleration, 2) + pow(b, 2));
	
	return this->getRadius() >= distance;
}
