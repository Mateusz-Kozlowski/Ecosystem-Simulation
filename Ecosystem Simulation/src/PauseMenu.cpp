#include "stdafx.h"
#include "PauseMenu.h"

using namespace gui;

PauseMenu::PauseMenu(const sf::VideoMode& video_mode, const sf::Font& font)
{
	this->videoMode = video_mode;
	this->font = font;

	//Init background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(video_mode.width),
			static_cast<float>(video_mode.height)
		)
	);
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//Init container
	this->container.setSize(
		sf::Vector2f(
			static_cast<float>(video_mode.width) / 4.f,
			static_cast<float>(video_mode.height)
		)
	);
	this->container.setFillColor(sf::Color(20, 20, 20, 220));
	this->container.setPosition(
		static_cast<float>(video_mode.width) / 2.f - this->container.getSize().x / 2.f,
		0.f
	);
}

PauseMenu::~PauseMenu()
{
	for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) delete it->second;

	// for (auto it = this->speedScaleSliders.begin(); it != this->speedScaleSliders.end(); ++it) delete it->second;
}

std::unordered_map<std::string, gui::Button*>& PauseMenu::getButtons()
{
	return this->buttons;
}

bool PauseMenu::isButtonClicked(const std::string& key)
{
	return this->buttons[key]->isClicked();
}

void PauseMenu::addButton(
	const std::string& key,
	float posY,
	float width, float height,
	int charSize, std::string text,
	sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
	sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
	float outlineThickness, short unsigned id)
{
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;

	this->buttons[key] = new gui::Button(
		x, posY, width, height,
		this->font, text, charSize,
		idleColor, hoverColor, pressedColor,
		outlineIdleColor, outlineHoverColor, outlinePressedColor,
		textIdleColor, textHoverColor, textPressedColor,
		outlineThickness, id
	);
}

void PauseMenu::addText(float posY, unsigned charSize, std::string text, sf::Color textColor)
{
	this->title.setFont(this->font);
	this->title.setCharacterSize(charSize);
	this->title.setString(text);
	this->title.setFillColor(textColor);
	this->title.setPosition(gui::p2pX(50, this->videoMode) - this->title.getGlobalBounds().width / 2.f, posY);
}

void PauseMenu::update(const sf::Vector2i& mousePosWindow)
{
	for (auto& i : this->buttons) i.second->update(mousePosWindow);
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);

	target.draw(this->title);

	for (auto& i : this->buttons) i.second->render(target);
}
