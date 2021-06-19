#pragma once

#include "Gui.h"

class ProgressBar
{
public:
	// constructor:
	ProgressBar(
		const sf::Vector2f& position, 
		const sf::Vector2f& size,
		const sf::Vector2f& range, float default_value,
		sf::Color background_color, sf::Color progress_color,
		bool correct_value_if_exceeds_range
	);

	// accessors:
	float getCurrentValue() const;

	const sf::Vector2f& getPosition() const;

	// mutators:
	void increaseValue(float change);

	void setValue(float value);

	void setPosition(const sf::Vector2f& position);

	void setProgressColor(const sf::Color& color);

	// other public methods:
	void render(sf::RenderTarget& target) const;

private:
	sf::RectangleShape backgroundRect;
	sf::RectangleShape progressRect;

	sf::Vector2f range;

	float value;

	bool correctValueIfExceedsRange;

	// initialization:
	void initBackground(const sf::Vector2f& position, const sf::Vector2f& size, sf::Color background_color);
	void initProgress(sf::Color progress_color);

	// private utilities:
	void avoidGoingBeyondRange();

	void updateProgressRectSize();
};
