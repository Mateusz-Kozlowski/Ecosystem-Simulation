#pragma once

class Individual
{
public:
	// constructor:
	Individual(float x, float y);

	void update(float dt);
	void render(sf::RenderTarget& target);

private:
	sf::CircleShape shape;
};

