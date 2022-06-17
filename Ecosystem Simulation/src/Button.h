#pragma once

#include "Gui.h"

namespace gui
{
	enum class ButtonState { BTN_IDLE = 0, BTN_HOVERED, BTN_PRESSED };

	class Button
	{
	public:
		Button(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, 
			const std::string& text, 
			unsigned charSize,
			const sf::Color& idleColor, 
			const sf::Color& hoverColor, 
			const sf::Color& pressedColor,
			const sf::Color& outlineIdleColor, 
			const sf::Color& outlineHoverColor, 
			const sf::Color& outlinePressedColor,
			const sf::Color& textIdleColor, 
			const sf::Color& textHoverColor, 
			const sf::Color& textPressedColor,
			float outlineThickness = 1.0f, 
			int id = 0
		);

		void update(const sf::Vector2f& mousePos);
		void render(sf::RenderTarget& target);

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

		void setPosition(const sf::Vector2f& newPos);

	private:
		// initialization:

		void initRect(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			float outlineThickness,
			const sf::Color& outlineIdleColor,
			const sf::Color& idleColor
		);
		void initText(const sf::Color& textIdleColor);

		// utils:

		void updateColors();

	private:
		bool m_clickBlockade;
		bool m_clicked;
		
		ButtonState m_state;
		
		unsigned short m_id;

		sf::RectangleShape m_rect;
		
		sf::Text m_text;

		sf::Color m_idleColor;
		sf::Color m_hoverColor;
		sf::Color m_pressedColor;

		sf::Color m_outlineIdleColor;
		sf::Color m_outlineHoverColor;
		sf::Color m_outlinePressedColor;

		sf::Color m_textIdleColor;
		sf::Color m_textHoverColor;
		sf::Color m_textPressedColor;
	};
}
