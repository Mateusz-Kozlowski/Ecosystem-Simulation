#pragma once

#include "Gui.h"

namespace gui
{
	enum class ButtonState { BTN_IDLE = 0, BTN_HOVERED, BTN_PRESSED };

	class Button
	{
	public:
		Button(
			float posX, float posY,
			float width, float height,
			const sf::Font& font, const std::string& text, int charSize,
			sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
			sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
			sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
			float outlineThickness = 1.f, int id = 0
		);

		bool isClicked() const;

		const std::string& getText() const;
		int getId() const;
		ButtonState getState() const;

		void setText(const std::string& text);
		void setId(int id);
		void setClickBlockade(bool blockade);
		void setIdle();
		void setHovered();

		void update(sf::Vector2i mousePosWindow);
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
