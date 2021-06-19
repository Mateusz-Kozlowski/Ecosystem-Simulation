#include "stdafx.h"
#include "ProgressBar.h"

// constructor:
ProgressBar::ProgressBar(
	const sf::Vector2f& pos, 
	const sf::Vector2f& size,
	const sf::Vector2f& range, float default_value, 
	sf::Color background_color, sf::Color progress_color,
	bool correct_value_if_exceeds_range)
	:	range(range), value(default_value), correctValueIfExceedsRange(correct_value_if_exceeds_range)
{
	this->initBackground(pos, size, background_color);
	this->initProgress(progress_color);
}

// accessors:
float ProgressBar::getCurrentValue() const
{
	return this->value;
}

const sf::Vector2f& ProgressBar::getPosition() const
{
	return this->backgroundRect.getPosition();
}

// mutators:
void ProgressBar::increaseValue(float change)
{
	this->value += change;

	if (this->correctValueIfExceedsRange) this->avoidGoingBeyondRange();

	this->updateProgressRectSize();
}

void ProgressBar::setValue(float value)
{
	this->value = value;

	if (this->correctValueIfExceedsRange) this->avoidGoingBeyondRange();

	this->updateProgressRectSize();
}

void ProgressBar::setPosition(const sf::Vector2f& position)
{
	this->backgroundRect.setPosition(position);
	this->progressRect.setPosition(position);
}

void ProgressBar::setProgressColor(const sf::Color& color)
{
	this->progressRect.setFillColor(color);
}

// other public methods:
void ProgressBar::render(sf::RenderTarget& target) const
{
	target.draw(this->backgroundRect);
	target.draw(this->progressRect);
}

// private methods:
// initialization:
void ProgressBar::initBackground(const sf::Vector2f& position, const sf::Vector2f& size, sf::Color background_color)
{
	this->backgroundRect.setPosition(position);
	this->backgroundRect.setSize(size);
	this->backgroundRect.setFillColor(background_color);
}

void ProgressBar::initProgress(sf::Color progress_color)
{
	this->progressRect.setPosition(this->backgroundRect.getPosition().x, this->backgroundRect.getPosition().y);
	this->updateProgressRectSize(); // sets size
	this->progressRect.setFillColor(progress_color);
}

// private utilities:
void ProgressBar::avoidGoingBeyondRange()
{
	if (this->value > this->range.y) this->value = this->range.y;

	else if (this->value < this->range.x) this->value = this->range.x;
}

void ProgressBar::updateProgressRectSize()
{
	this->progressRect.setSize(
		sf::Vector2f(
			this->backgroundRect.getSize().x * (this->value - this->range.x) / (this->range.y - this->range.x),
			this->backgroundRect.getSize().y
		)	
	);
}
