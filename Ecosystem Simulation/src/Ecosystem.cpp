#include "stdafx.h"
#include "Ecosystem.h"

Ecosystem::Ecosystem()
{
	sf::CircleShape c;

	for (int i = 0; i < 32; i++)
	{
		c.setRadius(float(i));
		c.setPointCount(8);
		c.setFillColor(sf::Color::Cyan);
		c.setPosition((200 * i) % 1920, (200 * i) % 1080);
		this->c.push_back(c);

		if (i % 4 == 0) this->c[i].setFillColor(sf::Color::Red);
		else if (i % 4 == 1) this->c[i].setFillColor(sf::Color::Cyan);
		else if (i % 4 == 2) this->c[i].setFillColor(sf::Color::Green);
		else if (i % 4 == 3) this->c[i].setFillColor(sf::Color::Yellow);
	}

	/*
	this->c[0].setPosition(50.f, 50.f);
	this->c[1].setPosition(3000.f, 3000.f);
	this->c[2].setPosition(3000.f, 50.f);
	this->c[3].setPosition(50.f, 3000.f);
	*/
}

void Ecosystem::render(sf::RenderTarget& target)
{

	sf::RectangleShape temp;
	temp.setSize(sf::Vector2f(100, 100));

	temp.setFillColor(sf::Color::Red);
	temp.setPosition(0, 0);
	target.draw(temp);

	temp.setFillColor(sf::Color::Blue);
	temp.setPosition(1920, 0);
	target.draw(temp);


	temp.setFillColor(sf::Color::Green);
	temp.setPosition(0, 1080);
	target.draw(temp);

	temp.setFillColor(sf::Color::Yellow);
	temp.setPosition(1920, 1080);
	target.draw(temp);

	for (auto& it : this->c) target.draw(it);
}
