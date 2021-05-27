#include "stdafx.h"
#include "ProgressBar.h"

// constructor:
ProgressBar::ProgressBar(
	float x, float y, 
	float width, float height, 
	const sf::Vector2f& range, float default_value, 
	sf::Color background_color, sf::Color progress_color)
	:	range(range), value(default_value)
{
	this->initBackground(x, y, width, height, background_color);
	this->initProgress(progress_color);
}

// accessors:
float ProgressBar::getCurrentValue() const
{
	return this->value;
}

// mutators:
void ProgressBar::increaseValue(float change)
{
	this->value += change;

	this->avoidGoingBeyondRange();

	this->updateProgressRectSize();
}

void ProgressBar::setPos(const sf::Vector2f& new_pos)
{
	this->backgroundRect.setPosition(new_pos);
	this->progressRect.setPosition(new_pos);
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
void ProgressBar::initBackground(float x, float y, float width, float height, sf::Color backgroundColor)
{
	this->backgroundRect.setPosition(x, y);
	this->backgroundRect.setSize(sf::Vector2f(width, height));
	this->backgroundRect.setFillColor(backgroundColor);
}

void ProgressBar::initProgress(sf::Color progressColor)
{
	this->progressRect.setPosition(this->backgroundRect.getPosition().x, this->backgroundRect.getPosition().y);
	this->updateProgressRectSize(); // sets size
	this->progressRect.setFillColor(progressColor);
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
