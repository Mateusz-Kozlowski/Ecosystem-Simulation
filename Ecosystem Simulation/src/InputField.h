#pragma once

#include "Gui.h"

namespace gui
{
	class InputField
	{
	public:
		InputField(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Font& font, const std::string& default_str, float char_size,
			const sf::Color& idle_color, const sf::Color& hovered_color, const sf::Color& pressed_color,
			const sf::Color& outline_idle_color, const sf::Color& outline_hovered_color, const sf::Color& outline_pressed_color,
			const sf::Color& text_idle_color, const sf::Color& text_hovered_color, const sf::Color& text_pressed_color,
			float outline_thickness, float cursor_width, float cursor_frequency,
			bool turned_on = true, int id = 0
		);

		// public methods:
		void update(float dt, const std::vector<sf::Event>& events, const sf::Vector2i& mouse_pos_window);
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
		sf::RectangleShape rect;
		
		const sf::Font& font;
		
		float charSize;
		
		sf::Text text;

		std::string input;
		
		sf::RectangleShape cursor;

		float stopwatch;
		float cursorFrequency;

		bool cursorIsRendered;

		std::unordered_map<std::string, std::unordered_map<std::string, sf::Color>> colors;

		bool turnedOnBlockade;

		bool turnedOn;
		bool m_HasBeenTurnedOn;

		int id;

		std::string state;

		bool textHasChanged;
		bool stateHasChanged;

		// private methods:
		// initialization:
		void initRect(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& idle_color,
			const sf::Color& outline_idle_color,
			float outline_thickness
		);
		void initText(
			const std::string& default_str,
			unsigned char_size,
			const sf::Color& text_idle_color
		);
		void initCursor(float cursor_width, const sf::Color& text_idle_color);

		void initColors(
			const sf::Color& idle_color, const sf::Color& hovered_color, const sf::Color& pressed_color,
			const sf::Color& outline_idle_color, const sf::Color& outline_hovered_color, const sf::Color& outline_pressed_color,
			const sf::Color& text_idle_color, const sf::Color& text_hovered_color, const sf::Color& text_pressed_color
		);

		// other private methods:
		void updateState(const std::vector<sf::Event>& events, const sf::Vector2i& mouse_position_window);
		void updateColors();
		void handleTextEntering(const std::vector<sf::Event>& events);
		void updateTextPosition();
		void updateCursorPosition();
		void updateCursorVisibility(float dt);
	};
}
