#pragma once

#include "TextBox.h"
#include "Button.h"

namespace gui
{
	class SaveAsPanel
	{
	public:
		SaveAsPanel(
			const sf::Vector2f& resolution, 
			float width, 
			const sf::Color& backgroundColor
		);
		
		void update(
			float dt, 
			const std::vector<sf::Event>& events, 
			const sf::Vector2i& mousePositionWindow
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
			unsigned charSize,
			const sf::Color& color
		);
		
		void initTextBox(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, 
			const std::string& defaultStr, 
			float charSize,
			const sf::Color& idleColor, 
			const sf::Color& hoveredColor, 
			const sf::Color& pressedColor,
			const sf::Color& outlineIdleColor, 
			const sf::Color& outlineHoveredColor, 
			const sf::Color& outlinePressedColor,
			const sf::Color& textIdleColor, 
			const sf::Color& textHoveredColor, 
			const sf::Color& textPressedColor,
			float outlineThickness, 
			float cursorWidth, 
			float cursorFrequency,
			bool turnedOn = true, 
			int id = 0
		);
		
		void addButton(
			const std::string& key,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, 
			const std::string& text, 
			unsigned charSize,
			sf::Color idleColor, 
			sf::Color hoverColor, 
			sf::Color pressedColor,
			sf::Color outlineIdleColor, 
			sf::Color outlineHoverColor, 
			sf::Color outlinePressedColor,
			sf::Color textIdleColor, 
			sf::Color textHoverColor, 
			sf::Color textPressedColor,
			float outlineThickness = 1.0f, 
			int id = 0
		);

	private:
		void initBackground(
			const sf::Vector2f& resolution,
			float width,
			const sf::Color& backgroundColor
		);

	private:
		typedef std::unique_ptr<gui::Button> BtnUniquePtr;

		const sf::Vector2f& m_resolution;

		sf::RectangleShape m_background;
		sf::Text m_text;

		std::unique_ptr<gui::TextBox> m_textBox;
		std::unordered_map<std::string, BtnUniquePtr> m_buttons;
	};
}
