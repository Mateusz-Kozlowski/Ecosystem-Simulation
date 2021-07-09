#include "stdafx.h"
#include "ProgressBar.h"

using namespace gui;

// constructors:
ProgressBar::ProgressBar()
	: valuesRange(sf::Vector2f(0.0f, 0.0f)),
	mOverRangeValuesAreCorrected(false),
	value(0.0f)
{

}

ProgressBar::ProgressBar(
	const sf::Vector2f& values_range,
	bool correct_over_range_values,
	float default_value,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& background_color,
	const sf::Color& progress_rect_color)
{
	this->valuesRange = values_range;
	this->mOverRangeValuesAreCorrected = correct_over_range_values;
	this->value = default_value;

	if (correct_over_range_values)
		this->avoidOverRangeValue();

	this->initBackground(position, size, background_color);
	this->initProgressRect(position, progress_rect_color);
}

// public methods:
void ProgressBar::render(sf::RenderTarget& target) const
{
	target.draw(this->background);
	target.draw(this->progressRect);
}

// accessors:
const sf::Vector2f& ProgressBar::getValuesRange() const
{
	return this->valuesRange;
}

bool ProgressBar::overRangeValuesAreCorrected() const
{
	return this->mOverRangeValuesAreCorrected;
}

float ProgressBar::getCurrentValue() const
{
	return this->value;
}

const sf::Vector2f& ProgressBar::getPosition() const
{
	return this->background.getPosition();
}

const sf::Vector2f& ProgressBar::getSize() const
{
	return this->background.getSize();
}

const sf::Color& ProgressBar::getBackgroundColor() const
{
	return this->background.getFillColor();
}

const sf::Color& ProgressBar::getProgressRectColor() const
{
	return this->progressRect.getFillColor();
}

// mutators:
void ProgressBar::setValuesRange(const sf::Vector2f& values_range)
{
	this->valuesRange = values_range;

	if (this->mOverRangeValuesAreCorrected) this->avoidOverRangeValue();

	this->updateProgressRectSize();
}

void ProgressBar::setCorrectingOverRangeValues(bool correct_over_range_values)
{
	this->mOverRangeValuesAreCorrected = correct_over_range_values;

	if (correct_over_range_values) this->avoidOverRangeValue();
}

void ProgressBar::setValue(float value)
{
	this->value = value;

	if (this->mOverRangeValuesAreCorrected) this->avoidOverRangeValue();

	this->updateProgressRectSize();
}

void ProgressBar::increaseValue(float value_increase)
{
	this->value += value_increase;

	if (this->mOverRangeValuesAreCorrected) this->avoidOverRangeValue();

	this->updateProgressRectSize();
}

void ProgressBar::decreaseValue(float value_decrease)
{
	this->value -= value_decrease;

	if (this->mOverRangeValuesAreCorrected) this->avoidOverRangeValue();

	this->updateProgressRectSize();
}

void ProgressBar::setPosition(const sf::Vector2f& position)
{
	this->background.setPosition(position);
	this->progressRect.setPosition(position);
}

void ProgressBar::setSize(const sf::Vector2f& size)
{
	this->background.setSize(size);

	this->updateProgressRectSize();
}

void ProgressBar::setBackgroundColor(const sf::Color& background_color)
{
	this->background.setFillColor(background_color);
}

void ProgressBar::setProgressRectColor(const sf::Color& progress_rect_color)
{
	this->progressRect.setFillColor(progress_rect_color);
}

// private methods:

// initialization:
void ProgressBar::initBackground(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& background_color)
{
	this->background.setPosition(position);
	this->background.setSize(size);
	this->background.setFillColor(background_color);
}

void ProgressBar::initProgressRect(const sf::Vector2f& position, const sf::Color& progress_rect_color)
{
	this->progressRect.setPosition(position);
	this->progressRect.setFillColor(progress_rect_color);
	this->updateProgressRectSize(); // sets size
}

// private utilities:
void ProgressBar::avoidOverRangeValue()
{
	if (this->value > this->valuesRange.y) this->value = this->valuesRange.y;

	else if (this->value < this->valuesRange.x) this->value = this->valuesRange.x;
}

void ProgressBar::updateProgressRectSize()
{
	float correctedValue = this->value;

	correctedValue = std::min(correctedValue, this->valuesRange.y);
	correctedValue = std::max(correctedValue, this->valuesRange.x);

	this->progressRect.setSize(
		sf::Vector2f(
			this->background.getSize().x * (correctedValue - this->valuesRange.x) / this->getRangeLength(),
			this->background.getSize().y
		)
	);
}

float ProgressBar::getRangeLength()
{
	return this->valuesRange.y - this->valuesRange.x;
}
