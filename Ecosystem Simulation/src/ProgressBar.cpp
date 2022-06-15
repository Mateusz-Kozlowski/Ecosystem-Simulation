#include "ProgressBar.h"

gui::ProgressBar::ProgressBar()
	: m_valuesRange()
	, m_overRangeValuesAreCorrected(false)
	, m_value(0.0)
	, m_background()
	, m_progressRect()
{
	
}

gui::ProgressBar::ProgressBar(
	const std::pair<double, double>& valuesRange,
	bool correctOverRangeValues,
	double defaultValue,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& backgroundColor,
	const sf::Color& progressRectColor)
	: m_valuesRange(valuesRange)
	, m_overRangeValuesAreCorrected(correctOverRangeValues)
	, m_value(defaultValue)
	, m_background(size)
	, m_progressRect()
{
	if (correctOverRangeValues)
	{
		avoidOverRangeValue();
	}

	initBackground(position, backgroundColor);
	initProgressRect(position, progressRectColor);
}

void gui::ProgressBar::render(sf::RenderTarget& target) const
{
	target.draw(m_background);
	target.draw(m_progressRect);
}

// accessors:

const std::pair<double, double>& gui::ProgressBar::getValuesRange() const
{
	return m_valuesRange;
}

bool gui::ProgressBar::overRangeValuesAreCorrected() const
{
	return m_overRangeValuesAreCorrected;
}

double gui::ProgressBar::getCurrentValue() const
{
	return m_value;
}

const sf::Vector2f& gui::ProgressBar::getPosition() const
{
	return m_background.getPosition();
}

const sf::Vector2f& gui::ProgressBar::getSize() const
{
	return m_background.getSize();
}

const sf::Color& gui::ProgressBar::getBackgroundColor() const
{
	return m_background.getFillColor();
}

const sf::Color& gui::ProgressBar::getProgressRectColor() const
{
	return m_progressRect.getFillColor();
}

// mutators:

void gui::ProgressBar::setValuesRange(
	const std::pair<double, double>& valuesRange)
{
	m_valuesRange = valuesRange;

	if (m_overRangeValuesAreCorrected)
	{
		avoidOverRangeValue();
	}

	updateProgressRectSize();
}

void gui::ProgressBar::setCorrectingOverRangeValues(
	bool correctOverRangeValues)
{
	m_overRangeValuesAreCorrected = correctOverRangeValues;

	if (correctOverRangeValues) 
	{
		avoidOverRangeValue();
	}
}

void gui::ProgressBar::setValue(double value)
{
	m_value = value;

	if (m_overRangeValuesAreCorrected)
	{
		avoidOverRangeValue();
	}

	updateProgressRectSize();
}

void gui::ProgressBar::increaseValue(double valueIncrease)
{
	m_value = m_value + valueIncrease; // += operator can cause a bug

	if (m_overRangeValuesAreCorrected)
	{
		avoidOverRangeValue();
	}

	updateProgressRectSize();
}

void gui::ProgressBar::decreaseValue(double valueDecrease)
{
	m_value = m_value - valueDecrease; // -= operator can cause a bug

	if (m_overRangeValuesAreCorrected)
	{
		avoidOverRangeValue();
	}

	updateProgressRectSize();
}

void gui::ProgressBar::setPosition(const sf::Vector2f& position)
{
	m_background.setPosition(position);
	m_progressRect.setPosition(position);
}

void gui::ProgressBar::setSize(const sf::Vector2f& size)
{
	m_background.setSize(size);

	updateProgressRectSize();
}

void gui::ProgressBar::setBackgroundColor(const sf::Color& backgroundColor)
{
	m_background.setFillColor(backgroundColor);
}

void gui::ProgressBar::setProgressRectColor(
	const sf::Color& progressRectColor)
{
	m_progressRect.setFillColor(progressRectColor);
}

// private methods:

// initialization:

void gui::ProgressBar::initBackground(
	const sf::Vector2f& position,
	const sf::Color& backgroundColor)
{
	m_background.setPosition(position);
	m_background.setFillColor(backgroundColor);
}

void gui::ProgressBar::initProgressRect(
	const sf::Vector2f& position, 
	const sf::Color& progressRectColor)
{
	m_progressRect.setPosition(position);
	m_progressRect.setFillColor(progressRectColor);
	updateProgressRectSize(); // sets m_size
}

// utils:

void gui::ProgressBar::avoidOverRangeValue()
{
	if (m_value > m_valuesRange.second)
	{
		m_value = m_valuesRange.second;
	}

	else if (m_value < m_valuesRange.first)
	{
		m_value = m_valuesRange.first;
	}
}

void gui::ProgressBar::updateProgressRectSize()
{
	double correctedValue = m_value;

	correctedValue = std::min(correctedValue, m_valuesRange.second);
	correctedValue = std::max(correctedValue, m_valuesRange.first);

	m_progressRect.setSize(
		sf::Vector2f(
			m_background.getSize().x * (correctedValue - m_valuesRange.first)
			/ getRangeLength(),
			m_background.getSize().y
		)
	);
}

double gui::ProgressBar::getRangeLength()
{
	return m_valuesRange.second - m_valuesRange.first;
}
