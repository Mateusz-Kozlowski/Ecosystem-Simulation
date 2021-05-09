#include "stdafx.h"
#include "Individual.h"

// constructor:
Individual::Individual(float x, float y)
{
	this->shape.setPosition(x, y);
	this->shape.setFillColor(sf::Color::Red);
	this->shape.setRadius(8.f);
	this->shape.setPointCount(16.f);
}

void Individual::update(float dt)
{
	this->shape.setPosition(this->shape.getPosition().x + 16.f * dt, this->shape.getPosition().y + 9.f * dt);
}

void Individual::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}
