#include "stdafx.h"
#include "Button.h"

using namespace gui;

gui::Button::Button(
	float posX, float posY,
	float width, float height,
	const sf::Font& font, const std::string& text, int charSize,
	sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
	sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
	float outlineThickness, int id)
{
	this->clickBlockade = false;
	this->clicked = false;
	this->state = ButtonState::BTN_IDLE;
	this->id = id;

	this->rect.setPosition(posX, posY);
	this->rect.setSize(sf::Vector2f(width, height));
	this->rect.setOutlineThickness(outlineThickness);
	this->rect.setOutlineColor(outlineIdleColor);
	this->rect.setFillColor(idleColor);

	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(charSize);
	this->text.setFillColor(textIdleColor);

	// centering the input:
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	this->text.setPosition(
		sf::Vector2f(
			this->rect.getPosition().x + this->rect.getSize().x / 2,
			this->rect.getPosition().y + this->rect.getSize().y / 2
		)
	);

	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->pressedColor = pressedColor;

	this->outlineIdleColor = outlineIdleColor;
	this->outlineHoverColor = outlineHoverColor;
	this->outlinePressedColor = outlinePressedColor;

	this->textIdleColor = textIdleColor;
	this->textHoverColor = textHoverColor;
	this->textPressedColor = textPressedColor;
}

bool gui::Button::isClicked() const
{
	return this->clicked;
}

const std::string& gui::Button::getText() const
{
	return this->text.getString();
}

int gui::Button::getId() const
{
	return this->id;
}

ButtonState gui::Button::getState() const
{
	return this->state;
}

void gui::Button::setText(const std::string& text)
{
	this->text.setString(text);
}

void gui::Button::setId(int id)
{
	this->id = id;
}

void gui::Button::setClickBlockade(bool blockade)
{
	this->clickBlockade = blockade;
}

void gui::Button::setIdle()
{
	this->state = ButtonState::BTN_IDLE;
	this->rect.setFillColor(this->idleColor);
	this->rect.setOutlineColor(this->outlineIdleColor);
	this->text.setFillColor(this->textIdleColor);
}

void gui::Button::setHovered()
{
	this->state = ButtonState::BTN_HOVERED;
	this->rect.setFillColor(this->hoverColor);
	this->rect.setOutlineColor(this->outlineHoverColor);
	this->text.setFillColor(this->textHoverColor);
}

void gui::Button::update(sf::Vector2i mousePosWindow)
{
	this->clicked = false;

	if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))))
		this->clickBlockade = false;

	if (this->state == ButtonState::BTN_PRESSED)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
				this->state = ButtonState::BTN_HOVERED;
			else
				this->state = ButtonState::BTN_IDLE;
		}
	}
	else if (this->state == ButtonState::BTN_HOVERED)
	{
		if (!this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
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
		if (this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) 
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

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->rect);
	target.draw(this->text);
}
