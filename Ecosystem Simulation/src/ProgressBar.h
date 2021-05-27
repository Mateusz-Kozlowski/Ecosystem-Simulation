#pragma once

#include "Gui.h"

class ProgressBar
{
public:
	// constructor:
	ProgressBar(
		float x, float y, 
		float width, float height,
		const sf::Vector2f& range, float default_value,
		sf::Color background_color, sf::Color progress_color
	);

	// accessors:
	float getCurrentValue() const;

	// mutators:
	void increaseValue(float change);

	void setPos(const sf::Vector2f& new_pos);

	void setProgressColor(const sf::Color& color);

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

	void updateProgressRectSize();
};
