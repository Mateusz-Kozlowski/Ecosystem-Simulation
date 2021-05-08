#include "stdafx.h"
#include "ScaleSlider.h"

gui::ScaleSlider::ScaleSlider(
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
		throw "ERROR: SPEEDScaleSlider::SPEEDScaleSlider: CANNOT LOAD AXIS TEXTURE FROM FILE";
	}

	// loading handle textures:
	if (!this->handleTextures["IDLE"].loadFromFile(handle_idle_path) ||
		!this->handleTextures["HOVERED"].loadFromFile(handle_hover_path) ||
		!this->handleTextures["PRESSED"].loadFromFile(handle_pressed_path))
	{
		throw "ERROR: SPEEDScaleSlider::SPEEDScaleSlider: CANNOT LOAD HANDLE TEXTURE FROM FILE";
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

float gui::ScaleSlider::getValue() const
{
	return this->value;
}

float gui::ScaleSlider::getMaxValue() const
{
	return this->maxValue;
}

void gui::ScaleSlider::setValue(float value)
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

void gui::ScaleSlider::setMinimizeToZero(bool minimizeToZero)
{
	this->minimizeToZero = minimizeToZero;
}

void gui::ScaleSlider::update(sf::Vector2i mousePosWindow)
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

void gui::ScaleSlider::render(sf::RenderTarget& target)
{
	target.draw(this->axes[this->state]);
	target.draw(this->handles[this->state]);
}
