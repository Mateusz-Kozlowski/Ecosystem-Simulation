#include "pch.h"
#include "SaveAsPanel.h"

gui::SaveAsPanel::SaveAsPanel(const sf::Vector2f& resolution, float width, const sf::Color& background_color)
	: resolution(resolution)
{
	this->initBackground(resolution, width, background_color);
}

// public methods:
void gui::SaveAsPanel::update(
	float dt, 
	const std::vector<sf::Event>& events, 
	const sf::Vector2i& mouse_position_window)
{
	this->textBox->update(dt, events, mouse_position_window);

	for (auto& button : this->buttons)
		button.second->update(mouse_position_window);
}

void gui::SaveAsPanel::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	target.draw(this->text);

	this->textBox->render(target);

	for (auto& button : this->buttons)
		button.second->render(target);
}

// accessors:
const gui::TextBox* gui::SaveAsPanel::getTextBox() const
{
	return this->textBox.get();
}

const gui::Button* gui::SaveAsPanel::getButton(const std::string& key) const
{
	return this->buttons.at(key).get();
}

// mutators:
void gui::SaveAsPanel::initCenteredText(
	float positionY, 
	const std::string& str, 
	const sf::Font& font, 
	unsigned char_size, 
	const sf::Color& color)
{
	this->text.setFont(font);
	this->text.setString(str);
	this->text.setCharacterSize(char_size);
	this->text.setFillColor(color);
	this->text.setOrigin(this->text.getGlobalBounds().width / 2.0f, 0.0f);
	this->text.setPosition(this->resolution.x / 2.0f, positionY);
}

void gui::SaveAsPanel::initTextBox(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Font& font, const std::string& default_str, float char_size,
	const sf::Color& idle_color, const sf::Color& hovered_color, const sf::Color& pressed_color,
	const sf::Color& outline_idle_color, const sf::Color& outline_hovered_color, const sf::Color& outline_pressed_color,
	const sf::Color& text_idle_color, const sf::Color& text_hovered_color, const sf::Color& text_pressed_color,
	float outline_thickness, float cursor_width, float cursor_frequency,
	bool turned_on, int id)
{
	this->textBox = std::make_unique<gui::TextBox>(
		position, 
		size,
		font, default_str, char_size,
		idle_color, hovered_color, pressed_color,
		outline_idle_color, outline_hovered_color, outline_pressed_color,
		text_idle_color, text_hovered_color, text_pressed_color,
		outline_thickness, cursor_width, cursor_frequency,
		turned_on, id
	);
}

void gui::SaveAsPanel::addButton(
	const std::string& key,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Font& font, const std::string& text, unsigned char_size,
	sf::Color idle_color, sf::Color hover_color, sf::Color pressed_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_pressed_color,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_pressed_color,
	float outline_thickness, int id)
{
	this->buttons[key] = std::make_unique<gui::Button>(
		position,
		size,
		font, text, char_size,
		idle_color, hover_color, pressed_color,
		outline_idle_color, outline_hover_color, outline_pressed_color,
		text_idle_color, text_hover_color, text_pressed_color,
		outline_thickness, id
	);
}

// private methods:

// initialization:
void gui::SaveAsPanel::initBackground(
	const sf::Vector2f& resolution, 
	float width, 
	const sf::Color& background_color)
{
	this->background.setSize(sf::Vector2f(width, resolution.y));
	this->background.setPosition(resolution.x / 2.0f - width / 2.0f, 0.0f);
	this->background.setFillColor(background_color);
}
