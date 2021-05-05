#include "stdafx.h"
#include "Gui.h"

float gui::p2pX(float perc, const sf::VideoMode& vm)
{
	return std::floor(static_cast<float>(vm.width) * (perc / 100.f));
}

float gui::p2pY(float perc, const sf::VideoMode& vm)
{
	return std::floor(static_cast<float>(vm.height) * (perc / 100.f));
}

unsigned gui::calcCharSize(const sf::VideoMode& vm, unsigned height_per_mille)
{
	return static_cast<unsigned>(vm.height * height_per_mille / 1000.f);
}

// BUTTON ============================================================
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

	// centering the text:
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

// SPEEDMODIFIER ============================================================
gui::Modifier::Modifier(
	float posX, float posY,
	float textureScale,
	float maxValue, bool maxOnLeft,
	const std::string& axis_idle_path, const std::string& handle_idle_path,
	const std::string& axis_hover_path, const std::string& handle_hover_path,
	const std::string& axis_pressed_path, const std::string& handle_pressed_path)
{
	// loading axis textures:
	if (!this->axisTextures["IDLE"].loadFromFile(axis_idle_path) ||
		!this->axisTextures["HOVERED"].loadFromFile(axis_hover_path) ||
		!this->axisTextures["PRESSED"].loadFromFile(axis_pressed_path))
	{
		throw "ERROR: SPEEDMODIFIER::SPEEDMODIFIER: CANNOT LOAD AXIS TEXTURE FROM FILE";
	}

	// loading handle textures:
	if (!this->handleTextures["IDLE"].loadFromFile(handle_idle_path) ||
		!this->handleTextures["HOVERED"].loadFromFile(handle_hover_path) ||
		!this->handleTextures["PRESSED"].loadFromFile(handle_pressed_path))
	{
		throw "ERROR: SPEEDMODIFIER::SPEEDMODIFIER: CANNOT LOAD HANDLE TEXTURE FROM FILE";
	}

	// setting smooth to the textures:
	for (auto& it : this->axisTextures)
		it.second.setSmooth(true);

	for (auto& it : this->handleTextures)
		it.second.setSmooth(true);

	// creating the axis:		
	for (auto& it : this->axisTextures)
	{
		sf::Sprite temp;

		temp.setTexture(it.second);

		temp.scale(
			sf::Vector2f(
				textureScale,
				textureScale
			)
		);

		temp.setPosition(
			posX - temp.getGlobalBounds().width / 2.f,
			posY - temp.getGlobalBounds().height / 2.f
		);

		this->axes[it.first] = temp;
	}

	// creating the handle:
	for (auto& it : this->handleTextures)
	{
		sf::Sprite temp;

		temp.setTexture(it.second);

		temp.scale(
			sf::Vector2f(
				textureScale,
				textureScale
			)
		);

		temp.scale(
			sf::Vector2f(
				this->axes["IDLE"].getGlobalBounds().height / temp.getGlobalBounds().height,
				this->axes["IDLE"].getGlobalBounds().height / temp.getGlobalBounds().height
			)
		);

		temp.setOrigin(0.f, 0.f);
		
		temp.setPosition(
			sf::Vector2f(
				this->axes["IDLE"].getGlobalBounds().left + this->axes["IDLE"].getGlobalBounds().width / 2.f - temp.getGlobalBounds().width / 2.f,
				this->axes["IDLE"].getGlobalBounds().top
			)
		);

		this->handles[it.first] = temp;
	}

	this->state = "IDLE";

	this->maxOnLeft = maxOnLeft;
	this->minimizeToZero = false;
	this->maxValue = maxValue;
	this->value = maxValue / 2.f;
}

float gui::Modifier::getValue() const
{
	return this->value;
}

float gui::Modifier::getMaxValue() const
{
	return this->maxValue;
}

void gui::Modifier::setValue(float value)
{
	this->value = value;

	float left = this->axes["IDLE"].getGlobalBounds().left;

	if (this->maxOnLeft)
	{
		for (auto& it : this->handles)
			it.second.setPosition(
				sf::Vector2f(
					left + (1.f - value / this->maxValue) * this->axes["IDLE"].getGlobalBounds().width - this->handles["IDLE"].getGlobalBounds().width / 2.f,
					it.second.getPosition().y
				)
			);
	}
	else
	{
		for (auto& it : this->handles)
			it.second.setPosition(
				sf::Vector2f(
					left + (value / this->maxValue) * this->axes["IDLE"].getGlobalBounds().width - this->handles["IDLE"].getGlobalBounds().width / 2.f,
					it.second.getPosition().y
				)
			);
	}
}

void gui::Modifier::setMinimizeToZero(bool minimizeToZero)
{
	this->minimizeToZero = minimizeToZero;
}

void gui::Modifier::update(sf::Vector2i mousePosWindow)
{
	if (this->state == "IDLE")
	{
		if (this->axes[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)) ||
			this->handles[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			this->state = "HOVERED";
			this->axes[state].setTexture(this->axisTextures["HOVERED"]);
			this->handles[state].setTexture(this->handleTextures["HOVERED"]);
		}
	}
	else if (this->state == "HOVERED")
	{
		if (!this->axes[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)) &&
			!this->handles[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			this->state = "IDLE";
			this->axes[this->state].setTexture(this->axisTextures["IDLE"]);
			this->handles[this->state].setTexture(this->handleTextures["IDLE"]);
		}
		else
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->state = "PRESSED";
				this->axes[this->state].setTexture(this->axisTextures["PRESSED"]);
				this->handles[this->state].setTexture(this->handleTextures["PRESSED"]);
			}
		}
	}
	else if (this->state == "PRESSED")
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (this->axes[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)) ||
				this->handles[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
			{
				this->state = "HOVERED";
				this->axes[this->state].setTexture(this->axisTextures["HOVERED"]);
				this->handles[this->state].setTexture(this->handleTextures["HOVERED"]);
			}
			else
			{
				this->state = "IDLE";
				this->axes[this->state].setTexture(this->axisTextures["IDLE"]);
				this->handles[this->state].setTexture(this->handleTextures["IDLE"]);
			}
		}
		else
		{
			float left = this->axes["IDLE"].getGlobalBounds().left;
			float right = left + this->axes["IDLE"].getGlobalBounds().width;

			if (mousePosWindow.x >= left && mousePosWindow.x <= right)
				for (auto& it : this->handles)
					it.second.setPosition(
						sf::Vector2f(
							mousePosWindow.x - this->handles[this->state].getGlobalBounds().width / 2.f,
							this->handles[this->state].getPosition().y
						)
					);
		}
	}

	// updating variables:
	float x = this->handles[this->state].getPosition().x + this->handles[this->state].getGlobalBounds().width / 2.f;

	float left = this->axes["IDLE"].getGlobalBounds().left;
	float right = left + this->axes["IDLE"].getGlobalBounds().width;

	if (this->maxOnLeft)
		this->value = this->maxValue * (right - x) / (right - left);
	else
		this->value = this->maxValue * (x - left) / (right - left);

	// setting very small values equal to 0:
	if (this->minimizeToZero && this->value < 0.1f * this->maxValue)
		this->setValue(0.f);
}

void gui::Modifier::render(sf::RenderTarget& target)
{
	target.draw(this->axes[this->state]);
	target.draw(this->handles[this->state]);
}
