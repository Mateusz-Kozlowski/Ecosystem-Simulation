#pragma once

class ProgressBar
{
public:
	// constructor:
	ProgressBar(
		float x, float y, 
		float width, float height,
		const sf::Vector2f& range, float defaultValue,
		sf::Color backgroundColor, sf::Color progressColor
	);

	// accessors:
	float getValue() const;

	// mutators:
	void increaseValue(float change);

	// other public methods:
	void render(sf::RenderTarget& target) const;

private:
	sf::RectangleShape backgroundRect;
	sf::RectangleShape progressRect;

	sf::Vector2f range;

	float value;

	// initialization:
	void initBackground(float x, float y, float width, float height, sf::Color backgroundColor);
	void initProgress(sf::Color progressColor);

	// private utilities:
	void avoidGoingBeyondRange();

	void updateProgressRect();
};
