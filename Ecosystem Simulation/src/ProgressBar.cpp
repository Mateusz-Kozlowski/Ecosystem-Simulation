#include "pch.h"
#include "ProgressBar.h"

gui::ProgressBar::ProgressBar()
	: m_valuesRange()
	, m_overRangeValuesAreCorrected(false)
	, m_value(0.0f)
	, m_background()
	, m_progressRect()
{
	
}

gui::ProgressBar::ProgressBar(
	const sf::Vector2f& valuesRange,
	bool correctOverRangeValues,
	float defaultValue,
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

const sf::Vector2f& gui::ProgressBar::getValuesRange() const
{
	return m_valuesRange;
}

bool gui::ProgressBar::overRangeValuesAreCorrected() const
{
	return m_overRangeValuesAreCorrected;
}

float gui::ProgressBar::getCurrentValue() const
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

void gui::ProgressBar::setValuesRange(const sf::Vector2f& valuesRange)
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

void gui::ProgressBar::setValue(float value)
{
	m_value = value;

	if (m_overRangeValuesAreCorrected)
	{
		avoidOverRangeValue();
	}

	updateProgressRectSize();
}

void gui::ProgressBar::increaseValue(float valueIncrease)
{
	m_value += valueIncrease;

	if (m_overRangeValuesAreCorrected)
	{
		avoidOverRangeValue();
	}

	updateProgressRectSize();
}

void gui::ProgressBar::decreaseValue(float valueDecrease)
{
	m_value -= valueDecrease;

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
	updateProgressRectSize(); // sets size
}

void gui::ProgressBar::avoidOverRangeValue()
{
	if (m_value > m_valuesRange.y)
	{
		m_value = m_valuesRange.y;
	}

	else if (m_value < m_valuesRange.x)
	{
		m_value = m_valuesRange.x;
	}
}

void gui::ProgressBar::updateProgressRectSize()
{
	float correctedValue = m_value;

	correctedValue = std::min(correctedValue, m_valuesRange.y);
	correctedValue = std::max(correctedValue, m_valuesRange.x);

	m_progressRect.setSize(
		sf::Vector2f(
			m_background.getSize().x * (correctedValue - m_valuesRange.x)
			/ getRangeLength(),
			m_background.getSize().y
		)
	);
}

float gui::ProgressBar::getRangeLength()
{
	return m_valuesRange.y - m_valuesRange.x;
}
