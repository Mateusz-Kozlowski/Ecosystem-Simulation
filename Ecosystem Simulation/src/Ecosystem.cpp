#include "stdafx.h"
#include "Ecosystem.h"

Ecosystem::Ecosystem(const sf::Vector2u& world_size, const std::string& name, unsigned border_size)
	: worldSize(world_size), name(name), borderSize(border_size)
{
	this->border.setSize(sf::Vector2f(3840.f, 2160.f));
	this->border.setFillColor(sf::Color(64, 64, 64));

	this->background.setSize(sf::Vector2f(3840.f - 2.f * border_size, 2160.f - 2.f * border_size));
	this->background.setFillColor(sf::Color(32, 32, 32));	
	this->background.setPosition(border_size, border_size);
}

// accessors:
const sf::Vector2u& Ecosystem::getWorldSize() const
{
	return this->worldSize;
}

const std::string& Ecosystem::getName() const
{
	return this->name;
}

unsigned Ecosystem::getBorderSize() const
{
	return this->borderSize;
}

// public methods:
void Ecosystem::update()
{

}

void Ecosystem::render(sf::RenderTarget& target)
{
	target.draw(this->border);
	target.draw(this->background);
}
