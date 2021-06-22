#pragma once

#include "Gui.h"

namespace gui
{
	enum class ButtonState { BTN_IDLE = 0, BTN_HOVERED, BTN_PRESSED };

	class Button
	{
	public:
		// constructor:
		Button(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, const std::string& text, unsigned char_size,
			sf::Color idle_color, sf::Color hover_color, sf::Color pressed_color,
			sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_pressed_color,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_pressed_color,
			float outline_thickness = 1.f, int id = 0
		);

		// accessors:
		bool isClicked() const;

		const std::string& getText() const;

		int getId() const;
		
		ButtonState getState() const;

		const sf::Vector2f& getPosition() const;

		// mutators:
		void setText(const std::string& text);

		void setId(int id);
		
		void setClickBlockade(bool blockade);
		
		void setIdle();
		
		void setHovered();

		void setPosition(const sf::Vector2f& new_pos);

		// other public methods:
		void update(sf::Vector2i mouse_pos_window);
		void render(sf::RenderTarget& target);

	private:
		bool clickBlockade;

		bool clicked;
		
		ButtonState state;
		
		unsigned short id;

		sf::RectangleShape rect;

		std::shared_ptr<sf::Font> font;
		
		sf::Text text;

		sf::Color idleColor, hoverColor, pressedColor;
		sf::Color outlineIdleColor, outlineHoverColor, outlinePressedColor;
		sf::Color textIdleColor, textHoverColor, textPressedColor;
	};
}
