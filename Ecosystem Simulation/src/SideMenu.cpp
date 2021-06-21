#include "stdafx.h"
#include "SideMenu.h"

using namespace gui;

// constructor/destructor:
SideMenu::SideMenu(
	const sf::Vector2f& position, 
	const sf::Vector2f& size,
	const sf::Color& background_color)
{
	this->initBackground(position, size, background_color);
}

SideMenu::~SideMenu()
{
	for (auto textureButton = this->textureButtons.begin(); textureButton != this->textureButtons.end(); ++textureButton)
		delete textureButton->second;
}

// accessors:
const sf::Vector2f& gui::SideMenu::getPosition() const
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

const sf::RectangleShape& gui::SideMenu::getBackground() const
{
	return this->background;
}

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

	// texts:
	for (auto& text : this->texts)
		text.setPosition(
			new_pos.x - oldPos.x + text.getPosition().x,
			new_pos.y - oldPos.y + text.getPosition().y
		);
}

void gui::SideMenu::addTextureButton(
	const std::string& key,
	const std::vector<std::pair<std::string, std::string>>& textures_path_and_keys,
	const std::string& key_of_default_texture,
	const sf::Vector2f& position, 
	const sf::Vector2f& size,
	int id)
{
	this->textureButtons[key] = new gui::TextureButton(
		textures_path_and_keys,
		key_of_default_texture,
		position,
		size
	);
}

void gui::SideMenu::addButton(
	const std::string& key, 
	const sf::Vector2f& position, 
	const sf::Vector2f& size, 
	int char_size, const sf::Font& font, const std::string& text,
	sf::Color idle_color, sf::Color hover_color, sf::Color pressed_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_pressed_color,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_pressed_color,
	float outlineThickness, 
	short unsigned id)
{
	this->buttons[key] = new gui::Button(
		position,
		size,
		font, text, char_size,
		idle_color, hover_color, pressed_color,
		outline_idle_color, outline_hover_color, outline_pressed_color,
		text_idle_color, text_hover_color, text_pressed_color,
		outlineThickness,
		id
	);
}

void gui::SideMenu::addScaleSlider(
	const std::string& key,
	const sf::Vector2f& position, 
	float textures_scale, 
	const std::pair<float, float>& range,
	const std::pair<float, float>& not_snapping_to_edges_range,
	float default_value, 
	const std::string& axis_idle_path, const std::string& handle_idle_path, 
	const std::string& axis_hover_path, const std::string& handle_hover_path, 
	const std::string& axis_pressed_path, const std::string& handle_pressed_path,
	const std::string& scale_function)
{
	this->scaleSliders[key] = new gui::ScaleSlider(
		position,
		textures_scale,
		range,
		not_snapping_to_edges_range,
		default_value,
		axis_idle_path, handle_idle_path,
		axis_hover_path, handle_hover_path,
		axis_pressed_path, handle_pressed_path,
		scale_function
	);
}

void gui::SideMenu::addCenteredText(
	float pos_y, 
	unsigned char_size, 
	const sf::Font& font,
	const std::string& text, 
	sf::Color color)
{
	this->texts.push_back(sf::Text());

	this->texts.back().setFont(font);
	this->texts.back().setString(text);
	this->texts.back().setCharacterSize(char_size);
	this->texts.back().setFillColor(color);

	auto rect = this->texts.back().getLocalBounds();

	this->texts.back().setPosition(
		this->background.getPosition().x + this->background.getSize().x / 2.f - rect.width / 2.f,
		pos_y
	);
}

void gui::SideMenu::setTextureOfTextureButton(const std::string& button_key, const std::string& texture_key)
{
	this->textureButtons[button_key]->setTexture(texture_key);
}

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

	for (const auto& textureButton : this->textureButtons) textureButton.second->render(target);
	
	for (const auto& button : this->buttons) button.second->render(target);

	for (const auto& scaleSlider : this->scaleSliders) scaleSlider.second->render(target);

	for (const auto& text : this->texts) target.draw(text);
}

// private methods:
// initialization:
void gui::SideMenu::initBackground(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color)
{
	this->background.setPosition(position);
	this->background.setSize(size);
	this->background.setFillColor(color);
}
