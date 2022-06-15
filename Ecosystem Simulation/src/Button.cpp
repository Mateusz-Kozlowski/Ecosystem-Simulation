#include "Button.h"

gui::Button::Button(
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
	: m_clickBlockade(false)
	, m_clicked(false)
	, m_state(ButtonState::BTN_IDLE)
	, m_id(id)
	, m_rect()
	, m_text(text, font, charSize)
	, m_idleColor(idleColor)
	, m_hoverColor(hoverColor)
	, m_pressedColor(pressedColor)
	, m_outlineIdleColor(outlineIdleColor)
	, m_outlineHoverColor(outlineHoverColor)
	, m_outlinePressedColor(outlinePressedColor)
	, m_textIdleColor(textIdleColor)
	, m_textHoverColor(textHoverColor)
	, m_textPressedColor(textPressedColor)
{
	initRect(position, size, outlineThickness, outlineIdleColor, idleColor);
	initText(textIdleColor);
}

void gui::Button::update(const sf::Vector2f& mousePos)
{
	m_clicked = false;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_clickBlockade = false;
	}

	if (m_state == ButtonState::BTN_PRESSED)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			const sf::Vector2f v2f = static_cast<sf::Vector2f>(mousePos);

			if (m_rect.getGlobalBounds().contains(v2f))
			{
				m_state = ButtonState::BTN_HOVERED;
			}
			else
			{
				m_state = ButtonState::BTN_IDLE;
			}
		}
	}
	else if (m_state == ButtonState::BTN_HOVERED)
	{
		const sf::Vector2f v2f = static_cast<sf::Vector2f>(mousePos);

		if (!m_rect.getGlobalBounds().contains(v2f))
		{
			m_state = ButtonState::BTN_IDLE;
		}
		else
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) 
				&& !m_clickBlockade)
			{
				m_clickBlockade = true;
				m_state = ButtonState::BTN_PRESSED;
				m_clicked = true;
			}
		}
	}
	else if (m_state == ButtonState::BTN_IDLE)
	{
		const sf::Vector2f v2f = static_cast<sf::Vector2f>(mousePos);

		if (m_rect.getGlobalBounds().contains(v2f))
		{
			m_state = ButtonState::BTN_HOVERED;
		}
	}

	updateColors();
}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(m_rect);
	target.draw(m_text);
}

// accessors:

bool gui::Button::isClicked() const
{
	return m_clicked;
}

const std::string& gui::Button::getText() const
{
	return m_text.getString();
}

int gui::Button::getId() const
{
	return m_id;
}

gui::ButtonState gui::Button::getState() const
{
	return m_state;
}

const sf::Vector2f& gui::Button::getPosition() const
{
	return m_rect.getPosition();
}

// mutators:

void gui::Button::setText(const std::string& text)
{
	m_text.setString(text);
}

void gui::Button::setId(int id)
{
	m_id = id;
}

void gui::Button::setClickBlockade(bool blockade)
{
	m_clickBlockade = blockade;
}

void gui::Button::setIdle()
{
	m_state = ButtonState::BTN_IDLE;
	m_rect.setFillColor(m_idleColor);
	m_rect.setOutlineColor(m_outlineIdleColor);
	m_text.setFillColor(m_textIdleColor);
}

void gui::Button::setHovered()
{
	m_state = ButtonState::BTN_HOVERED;
	m_rect.setFillColor(m_hoverColor);
	m_rect.setOutlineColor(m_outlineHoverColor);
	m_text.setFillColor(m_textHoverColor);
}

void gui::Button::setPosition(const sf::Vector2f& newPos)
{
	m_rect.setPosition(
		sf::Vector2f(
			newPos.x + m_rect.getOutlineThickness(),
			newPos.y + m_rect.getOutlineThickness()
		)
	);

	m_text.setPosition(
		sf::Vector2f(
			m_rect.getPosition().x + m_rect.getSize().x / 2,
			m_rect.getPosition().y + m_rect.getSize().y / 2
		)
	);
}

// private methods:

// initialization:

void gui::Button::initRect(
	const sf::Vector2f& position, 
	const sf::Vector2f& size, 
	float outlineThickness,
	const sf::Color& outlineIdleColor,
	const sf::Color& idleColor)
{
	m_rect.setPosition(
		sf::Vector2f(
			position.x + outlineThickness,
			position.y + outlineThickness
		)
	);
	m_rect.setSize(
		sf::Vector2f(
			size.x - 2.0f * outlineThickness,
			size.y - 2.0f * outlineThickness
		)
	);
	m_rect.setOutlineThickness(outlineThickness);
	m_rect.setOutlineColor(outlineIdleColor);
	m_rect.setFillColor(idleColor);
}

void gui::Button::initText(const sf::Color& textIdleColor)
{
	m_text.setFillColor(textIdleColor);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(
		textRect.left + textRect.width / 2.0f, 
		textRect.top + textRect.height / 2.0f
	);

	m_text.setPosition(
		sf::Vector2f(
			m_rect.getPosition().x + m_rect.getSize().x / 2,
			m_rect.getPosition().y + m_rect.getSize().y / 2
		)
	);
}

// utils:

void gui::Button::updateColors()
{
	if (m_state == ButtonState::BTN_PRESSED)
	{
		m_rect.setFillColor(m_pressedColor);
		m_rect.setOutlineColor(m_outlinePressedColor);
		m_text.setFillColor(m_textPressedColor);
	}
	else if (m_state == ButtonState::BTN_HOVERED)
	{
		m_rect.setFillColor(m_hoverColor);
		m_rect.setOutlineColor(m_outlineHoverColor);
		m_text.setFillColor(m_textHoverColor);
	}
	else if (m_state == ButtonState::BTN_IDLE)
	{
		m_rect.setFillColor(m_idleColor);
		m_rect.setOutlineColor(m_outlineIdleColor);
		m_text.setFillColor(m_textIdleColor);
	}
}
