#pragma once

class Fruit
{
public:
	Fruit(
		float energy,
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	Fruit(const char* filePath);
	
	void saveToFile(const char* filePath) const;
	void loadFromFile(const char* filePath);

	void render(sf::RenderTarget& target) const;

	bool isCoveredByMouse(const sf::Vector2f& mousePosView) const;

	// accessors:

	float getEnergy() const;

	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	const sf::Color& getColor() const;

	// mutators:

	void setEnergy(float energy);
	void increaseEnergy(float increase);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& newPosition);

	void setRandomPosition(
		const sf::Vector2f& worldSize, 
		float bordersThickness
	);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

private:
	void initShape(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	
private:
	float m_energy;

	sf::CircleShape m_shape;
};
