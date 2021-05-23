#include "stdafx.h"
#include "SideMenu.h"

using namespace gui;

// constructor/destructor:
SideMenu::SideMenu(
	sf::Font& font, 
	const sf::Vector2f& pos, 
	const sf::Vector2f& size,
	const sf::Color& background_color)
	: font(font)
{
	this->initBackground(pos, size, background_color);
}

SideMenu::~SideMenu()
{
	for (auto textureButton = this->textureButtons.begin(); textureButton != this->textureButtons.end(); ++textureButton)
		delete textureButton->second;
}

// accessors:
const sf::Vector2f& gui::SideMenu::getPos() const
{
	return this->background.getPosition();
}

const sf::Vector2f& gui::SideMenu::getSize() const
{
	return this->background.getSize();
}

const std::unordered_map<std::string, gui::TextureButton*>& gui::SideMenu::getTextureButtons() const
{
	return this->textureButtons;
}

const std::unordered_map<std::string, gui::Button*>& gui::SideMenu::getButtons() const
{
	return this->buttons;
}

const std::unordered_map<std::string, gui::ScaleSlider*>& gui::SideMenu::getScaleSliders() const
{
	return this->scaleSliders;
}

bool SideMenu::hasTextureButtonBeenClicked(const std::string& key)
{
	return this->textureButtons[key]->hasBeenClicked();
}

/*
void SideMenu::addButton(
	const std::string& key,
	float pos_y,
	float width, float height,
	int charSize, std::string text,
	sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
	sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
	float outlineThickness, short unsigned id)
{
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;

	this->buttons[key] = new gui::Button(
		x, pos_y, width, height,
		this->font, text, charSize,
		idleColor, hoverColor, pressedColor,
		outlineIdleColor, outlineHoverColor, outlinePressedColor,
		textIdleColor, textHoverColor, textPressedColor,
		outlineThickness, id
	);
}
*/

// mutators:
void gui::SideMenu::setPosition(const sf::Vector2f& new_pos)
{
	sf::Vector2f oldPos = this->background.getPosition();
	
	this->background.setPosition(new_pos);

	// texture buttons:
	for (auto& textureButton : this->textureButtons)
		textureButton.second->setPosition(
			sf::Vector2f(
				new_pos.x - oldPos.x + textureButton.second->getPosition().x,
				new_pos.y - oldPos.y + textureButton.second->getPosition().y
			)
		);

	// buttons:
	for (auto& button : this->buttons)
		button.second->setPosition(
			sf::Vector2f(
				new_pos.x - oldPos.x + button.second->getPosition().x,
				new_pos.y - oldPos.y + button.second->getPosition().y
			)
		);

	// scale sliders:
	for (auto& scaleSlider: this->scaleSliders)
		scaleSlider.second->setPosition(
			sf::Vector2f(
				new_pos.x - oldPos.x + scaleSlider.second->getPosition().x,
				new_pos.y - oldPos.y + scaleSlider.second->getPosition().y
			)
		);
}

void gui::SideMenu::addTextureButton(
	const std::string& key,
	const std::vector<std::pair<std::string, std::string>>& textures_path_and_keys,
	const std::string& key_of_default_texture,
	float pos_x, float pos_y, 
	float width, float height, 
	int id)
{
	this->textureButtons[key] = new gui::TextureButton(
		textures_path_and_keys,
		key_of_default_texture,
		pos_x, pos_y,
		width, height
	);
}

void gui::SideMenu::addButton(
	const std::string& key, 
	const sf::Vector2f& pos, 
	float width, float height, 
	int charSize, std::string text, 
	sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor, 
	sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor, 
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor, 
	float outlineThickness, 
	short unsigned id)
{
	this->buttons[key] = new gui::Button(
		pos.x, pos.y,
		width, height,
		this->font, text, charSize,
		idleColor, hoverColor, pressedColor,
		outlineIdleColor, outlineHoverColor, outlinePressedColor,
		textIdleColor, textHoverColor, textPressedColor,
		outlineThickness,
		id
	);
}

void gui::SideMenu::addScaleSlider(
	const std::string& key,
	float pos_x, float pos_y, 
	float textures_scale, 
	const std::pair<float, float>& range, 
	float default_value, 
	const std::string& axis_idle_path, const std::string& handle_idle_path, 
	const std::string& axis_hover_path, const std::string& handle_hover_path, 
	const std::string& axis_pressed_path, const std::string& handle_pressed_path)
{
	this->scaleSliders[key] = new gui::ScaleSlider(
		pos_x, pos_y,
		textures_scale,
		range,
		default_value,
		axis_idle_path, handle_idle_path,
		axis_hover_path, handle_hover_path,
		axis_pressed_path, handle_pressed_path
	);
}

void gui::SideMenu::setTextureOfTextureButton(const std::string& button_key, const std::string& texture_key)
{
	this->textureButtons[button_key]->setTexture(texture_key);
}

/*
void SideMenu::addText(float pos_y, unsigned charSize, std::string text, sf::Color textColor)
{
	this->title.setFont(this->font);
	this->title.setCharacterSize(charSize);
	this->title.setString(text);
	this->title.setFillColor(textColor);
	this->title.setPosition(gui::p2pX(50, this->videoMode) - this->title.getGlobalBounds().width / 2.f, pos_y);
}
*/

// other public methods:
void SideMenu::update(const sf::Vector2i& mouse_pos_window, const std::vector<sf::Event>& events)
{
	for (auto& textureButton : this->textureButtons) 
		textureButton.second->update(mouse_pos_window, events);

	for (auto& button : this->buttons)
		button.second->update(mouse_pos_window);

	for (auto& scaleSlider : this->scaleSliders)
		scaleSlider.second->update(mouse_pos_window);
}

void SideMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	for (auto& textureButton : this->textureButtons) textureButton.second->render(target);
	
	for (auto& button : this->buttons) button.second->render(target);

	for (auto& scaleSlider : this->scaleSliders) scaleSlider.second->render(target);
}

// private methods:
// initialization:
void gui::SideMenu::initBackground(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
	this->background.setPosition(pos);
	this->background.setSize(size);
	this->background.setFillColor(color);
}
