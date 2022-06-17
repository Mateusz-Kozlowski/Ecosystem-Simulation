#include "ProgressBar.h"

gui::IntProgressBar::IntProgressBar()
	: m_valuesRange()
	, m_value(0)
	, m_background()
	, m_progressRect()
{
	
}

gui::IntProgressBar::IntProgressBar(
	const sf::Vector2i& valuesRange,
	int defaultValue,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& backgroundColor,
	const sf::Color& progressRectColor)
	: m_valuesRange(valuesRange)
	, m_value(defaultValue)
	, m_background(size)
	, m_progressRect()
{
	initBackground(position, backgroundColor);
	initProgressRect(position, progressRectColor);
}

void gui::IntProgressBar::render(sf::RenderTarget& target) const
{
	target.draw(m_background);
	target.draw(m_progressRect);
}

// accessors:

const sf::Vector2i& gui::IntProgressBar::getValuesRange() const
{
	return m_valuesRange;
}

int gui::IntProgressBar::getCurrentValue() const
{
	return m_value;
}

const sf::Vector2f& gui::IntProgressBar::getPosition() const
{
	return m_background.getPosition();
}

const sf::Vector2f& gui::IntProgressBar::getSize() const
{
	return m_background.getSize();
}

const sf::Color& gui::IntProgressBar::getBackgroundColor() const
{
	return m_background.getFillColor();
}

const sf::Color& gui::IntProgressBar::getProgressRectColor() const
{
	return m_progressRect.getFillColor();
}

// mutators:

void gui::IntProgressBar::setValuesRange(const sf::Vector2i& valuesRange)
{
	m_valuesRange = valuesRange;

	updateProgressRectSize();
}

void gui::IntProgressBar::setValue(int value)
{
	m_value = value;

	updateProgressRectSize();
}

void gui::IntProgressBar::increaseValue(int valueIncrease)
{
	m_value = m_value + valueIncrease; // += operator can cause a bug

	updateProgressRectSize();
}

void gui::IntProgressBar::decreaseValue(int valueDecrease)
{
	m_value = m_value - valueDecrease; // -= operator can cause a bug

	updateProgressRectSize();
}

void gui::IntProgressBar::setPosition(const sf::Vector2f& position)
{
	m_background.setPosition(position);
	m_progressRect.setPosition(position);
}

void gui::IntProgressBar::setSize(const sf::Vector2f& size)
{
	m_background.setSize(size);

	updateProgressRectSize();
}

void gui::IntProgressBar::setBackgroundColor(const sf::Color& backgroundColor)
{
	m_background.setFillColor(backgroundColor);
}

void gui::IntProgressBar::setProgressRectColor(
	const sf::Color& progressRectColor)
{
	m_progressRect.setFillColor(progressRectColor);
}

// private methods:

// initialization:

void gui::IntProgressBar::initBackground(
	const sf::Vector2f& position,
	const sf::Color& backgroundColor)
{
	m_background.setPosition(position);
	m_background.setFillColor(backgroundColor);
}

void gui::IntProgressBar::initProgressRect(
	const sf::Vector2f& position, 
	const sf::Color& progressRectColor)
{
	m_progressRect.setPosition(position);
	m_progressRect.setFillColor(progressRectColor);
	updateProgressRectSize(); // sets m_size
}

// utils:

void gui::IntProgressBar::updateProgressRectSize()
{
	int correctedValue = m_value;

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

int gui::IntProgressBar::getRangeLength()
{
	return m_valuesRange.y - m_valuesRange.x;
}
