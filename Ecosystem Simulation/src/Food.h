#pragma once

class Food
{
public:
	// constructor:
	Food(float x, float y);

	// accessors:
	const sf::Vector2f& getPosition() const;

	// other public methods:
	void render(sf::RenderTarget& target);

private:
	sf::CircleShape shape;
};
