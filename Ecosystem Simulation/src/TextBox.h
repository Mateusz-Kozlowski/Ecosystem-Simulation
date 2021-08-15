#pragma once

#include "Gui.h"

namespace gui
{
	class TextBox
	{
	public:
		TextBox(
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
		
		void update(
			float dt, 
			const std::vector<sf::Event>& events, 
			const sf::Vector2i& mousePosWindow
		);
		void render(sf::RenderTarget& target);

		// accessors:

		int getId() const;

		const std::string& getInput() const;

		bool isTurnedOn() const;
		bool hasBeenTurnedOn() const;

		// mutators:

		void turnOn();
		void turnOff();

		void setString(const std::string& string);

	private:
		void initRect(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& idleColor,
			const sf::Color& outlineIdlColor,
			float outlineThickness
		);
		void initText(const sf::Color& textIdleColor);
		void initCursor(const sf::Color& textIdleColor);

		void initColors(
			const sf::Color& idleColor, 
			const sf::Color& hoveredColor, 
			const sf::Color& pressedColor,
			const sf::Color& outlineIdleColor, 
			const sf::Color& outlineHoveredColor, 
			const sf::Color& outlinePressedColor,
			const sf::Color& textIdleColor, 
			const sf::Color& textHoveredColor, 
			const sf::Color& textPressedColor
		);
		
		void updateState(
			const std::vector<sf::Event>& events, 
			const sf::Vector2i& mousePositionWindow);
		void updateColors();
		void handleTextEntering(const std::vector<sf::Event>& events);
		void updateTextPosition();
		void updateCursorPosition();
		void updateCursorVisibility(float dt);

	private:
		typedef std::unordered_map<std::string, sf::Color> Str2ColorMap;

		sf::RectangleShape m_rect;
		
		sf::Text m_text;

		std::string m_input;
		
		sf::RectangleShape m_cursor;

		float m_stopwatch;
		float m_cursorFrequency;

		bool m_cursorIsRendered;

		std::unordered_map<std::string, Str2ColorMap> m_colors;

		bool m_turnedOnBlockade;

		bool m_turnedOn;
		bool m_hasBeenTurnedOn;

		int m_id;

		std::string m_state;

		bool m_textHasChanged;
		bool m_stateHasChanged;
	};
}
