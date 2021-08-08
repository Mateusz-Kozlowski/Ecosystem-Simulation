#pragma once

#include "TextBox.h"
#include "Button.h"

namespace gui
{
	class SaveAsPanel
	{
	public:
		SaveAsPanel(const sf::Vector2f& resolution, float width, const sf::Color& background_color);

		// public methods:
		void update(
			float dt, 
			const std::vector<sf::Event>& events, 
			const sf::Vector2i& mouse_position_window
		);
		void render(sf::RenderTarget& target);

		// accessors:
		const gui::TextBox* getTextBox() const;
		const gui::Button* getButton(const std::string& key) const;

		// mutators:
		void initCenteredText(
			float positionY,
			const std::string& str,
			const sf::Font& font,
			unsigned char_size,
			const sf::Color& color
		);
		
		void initTextBox(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, const std::string& default_str, float char_size,
			const sf::Color& idle_color, const sf::Color& hovered_color, const sf::Color& pressed_color,
			const sf::Color& outline_idle_color, const sf::Color& outline_hovered_color, const sf::Color& outline_pressed_color,
			const sf::Color& text_idle_color, const sf::Color& text_hovered_color, const sf::Color& text_pressed_color,
			float outline_thickness, float cursor_width, float cursor_frequency,
			bool turned_on = true, int id = 0
		);
		
		void addButton(
			const std::string& key,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, const std::string& text, unsigned char_size,
			sf::Color idle_color, sf::Color hover_color, sf::Color pressed_color,
			sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_pressed_color,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_pressed_color,
			float outline_thickness = 1.0f, int id = 0
		);

	private:
		const sf::Vector2f& resolution;

		sf::RectangleShape background;
		sf::Text text;

		std::unique_ptr<gui::TextBox> textBox;
		std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

		// private methods:
		// initialization:
		void initBackground(
			const sf::Vector2f& resolution, 
			float width, 
			const sf::Color& background_color
		);
	};
}
