#include "stdafx.h"
#include "Button.h"

using namespace gui;

// constructor:
Button::Button(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Font& font, const std::string& text, unsigned char_size,
	sf::Color idle_color, sf::Color hover_color, sf::Color pressed_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_pressed_color,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_pressed_color,
	float outline_thickness, int id)
{
	this->clickBlockade = false;
	this->clicked = false;
	this->state = ButtonState::BTN_IDLE;
	this->id = id;

	this->rect.setPosition(position);
	this->rect.setSize(size);
	this->rect.setOutlineThickness(outline_thickness);
	this->rect.setOutlineColor(outline_idle_color);
	this->rect.setFillColor(idle_color);

	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(char_size);
	this->text.setFillColor(text_idle_color);

	// centering the input:
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	this->text.setPosition(
		sf::Vector2f(
			this->rect.getPosition().x + this->rect.getSize().x / 2,
			this->rect.getPosition().y + this->rect.getSize().y / 2
		)
	);

	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->pressedColor = pressed_color;

	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlinePressedColor = outline_pressed_color;

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textPressedColor = text_pressed_color;
}

// accessors:
bool Button::isClicked() const
{
	return this->clicked;
}

const std::string& Button::getText() const
{
	return this->text.getString();
}

int Button::getId() const
{
	return this->id;
}

ButtonState Button::getState() const
{
	return this->state;
}

const sf::Vector2f& Button::getPosition() const
{
	return this->rect.getPosition();
}

// mutators:
void Button::setText(const std::string& text)
{
	this->text.setString(text);
}

void Button::setId(int id)
{
	this->id = id;
}

void Button::setClickBlockade(bool blockade)
{
	this->clickBlockade = blockade;
}

void Button::setIdle()
{
	this->state = ButtonState::BTN_IDLE;
	this->rect.setFillColor(this->idleColor);
	this->rect.setOutlineColor(this->outlineIdleColor);
	this->text.setFillColor(this->textIdleColor);
}

void Button::setHovered()
{
	this->state = ButtonState::BTN_HOVERED;
	this->rect.setFillColor(this->hoverColor);
	this->rect.setOutlineColor(this->outlineHoverColor);
	this->text.setFillColor(this->textHoverColor);
}

void Button::setPosition(const sf::Vector2f& new_pos)
{
	this->rect.setPosition(new_pos);

	this->text.setPosition(
		sf::Vector2f(
			this->rect.getPosition().x + this->rect.getSize().x / 2,
			this->rect.getPosition().y + this->rect.getSize().y / 2
		)
	);
}

// other public methods:
void Button::update(const sf::Vector2i& mouse_pos_window)
{
	this->clicked = false;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) this->clickBlockade = false;

	if (this->state == ButtonState::BTN_PRESSED)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos_window)))
				this->state = ButtonState::BTN_HOVERED;
			else
				this->state = ButtonState::BTN_IDLE;
		}
	}
	else if (this->state == ButtonState::BTN_HOVERED)
	{
		if (!this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos_window)))
			this->state = ButtonState::BTN_IDLE;
		else
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->clickBlockade)
			{
				this->clickBlockade = true;
				this->state = ButtonState::BTN_PRESSED;
				this->clicked = true;
			}
		}
	}
	else if (this->state == ButtonState::BTN_IDLE)
	{
		if (this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos_window)))
			this->state = ButtonState::BTN_HOVERED;
	}

	if (this->state == ButtonState::BTN_PRESSED)
	{
		this->rect.setFillColor(this->pressedColor);
		this->rect.setOutlineColor(this->outlinePressedColor);
		this->text.setFillColor(this->textPressedColor);
	}
	else if (this->state == ButtonState::BTN_HOVERED)
	{
		this->rect.setFillColor(this->hoverColor);
		this->rect.setOutlineColor(this->outlineHoverColor);
		this->text.setFillColor(this->textHoverColor);
	}
	else if (this->state == ButtonState::BTN_IDLE)
	{
		this->rect.setFillColor(this->idleColor);
		this->rect.setOutlineColor(this->outlineIdleColor);
		this->text.setFillColor(this->textIdleColor);
	}
}

void Button::render(sf::RenderTarget& target)
{
	target.draw(this->rect);
	target.draw(this->text);
}
