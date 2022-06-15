#include "SaveAsPanel.h"

gui::SaveAsPanel::SaveAsPanel(
	const sf::Vector2f& resolution, 
	float width, 
	const sf::Color& backgroundColor)
	: m_resolution(resolution)
	, m_background(sf::Vector2f(width, resolution.y))
	, m_text()
	, m_textBox()
	, m_buttons()
{
	initBackground(resolution, width, backgroundColor);
}

void gui::SaveAsPanel::update(
	float dt,
	const std::vector<sf::Event>& events, 
	const sf::Vector2i& mousePositionWindow)
{
	m_textBox->update(dt, events, mousePositionWindow);

	for (auto& button : m_buttons)
	{
		button.second->update(static_cast<sf::Vector2f>(mousePositionWindow));
	}
}

void gui::SaveAsPanel::render(sf::RenderTarget& target)
{
	target.draw(m_background);
	target.draw(m_text);

	m_textBox->render(target);

	for (auto& button : m_buttons)
	{
		button.second->render(target);
	}
}

// accessors:

const gui::TextBox* gui::SaveAsPanel::getTextBox() const
{
	return m_textBox.get();
}

const gui::Button* gui::SaveAsPanel::getButton(
	const std::string& key) const
{
	return m_buttons.at(key).get();
}

// mutators:

void gui::SaveAsPanel::initCenteredText(
	float positionY, 
	const std::string& str, 
	const sf::Font& font, 
	unsigned charSize, 
	const sf::Color& color)
{
	m_text.setFont(font);
	m_text.setString(str);
	m_text.setCharacterSize(charSize);
	m_text.setFillColor(color);
	m_text.setOrigin(
		m_text.getGlobalBounds().width / 2.0f, 
		0.0f
	);
	m_text.setPosition(m_resolution.x / 2.0f, positionY);
}

void gui::SaveAsPanel::initTextBox(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Font& font, 
	const std::string& defaultStr, 
	float charSize,
	const sf::Color& idleColor, 
	const sf::Color& hoveredColor, 
	const sf::Color& pressedColor,
	const sf::Color& outlineIdleColor, 
	const sf::Color& outlineHoveredColor, 
	const sf::Color& outlinePressedColor,
	const sf::Color& textIdleColor, 
	const sf::Color& textHoveredColor, 
	const sf::Color& textPressedColor,
	float outlineThickness, 
	float cursorWidth, 
	float cursorFrequency,
	bool turnedOn, 
	int id)
{
	m_textBox = std::make_unique<gui::TextBox>(
		position, 
		size,
		font, defaultStr, charSize,
		idleColor, hoveredColor, pressedColor,
		outlineIdleColor, outlineHoveredColor, outlinePressedColor,
		textIdleColor, textHoveredColor, textPressedColor,
		outlineThickness, cursorWidth, cursorFrequency,
		turnedOn, id
	);
}

void gui::SaveAsPanel::addButton(
	const std::string& key,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Font& font, 
	const std::string& text, 
	unsigned charSize,
	sf::Color idleColor, 
	sf::Color hoverColor, 
	sf::Color pressedColor,
	sf::Color outlineIdleColor, 
	sf::Color outlineHoverColor, 
	sf::Color outlinePressedColor,
	sf::Color textIdleColor, 
	sf::Color textHoverColor, 
	sf::Color textPressedColor,
	float outlineThickness, 
	int id)
{
	m_buttons[key] = std::make_unique<gui::Button>(
		position,
		size,
		font, text, charSize,
		idleColor, hoverColor, pressedColor,
		outlineIdleColor, outlineHoverColor, outlinePressedColor,
		textIdleColor, textHoverColor, textPressedColor,
		outlineThickness, id
	);
}

// private methods:

void gui::SaveAsPanel::initBackground(
	const sf::Vector2f& resolution, 
	float width, 
	const sf::Color& backgroundColor)
{
	m_background.setSize(sf::Vector2f(width, resolution.y));
	m_background.setPosition(resolution.x / 2.0f - width / 2.0f, 0.0f);
	m_background.setFillColor(backgroundColor);
}
