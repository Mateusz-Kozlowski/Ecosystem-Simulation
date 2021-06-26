#pragma once

#include "Gui.h"

namespace gui
{
	class InputField
	{
	public:
		// constructor:
		InputField(
			float pos_x, float pos_y, 
			float width, float height,
			const sf::Font& font, const std::string& default_text, float char_size,
			sf::Color color, sf::Color text_color, sf::Color outline_color,
			float outline_thickness, float cursor_width,
			bool active = true,
			int id = 0
		);

		// accessors:
		int getId() const;

		const std::string& getInput() const;

		bool isActive() const;

		bool hasBeenClicked(
			const sf::Vector2f& mouse_pos_window,
			const std::vector<sf::Event>& events
		) const;

		// mutators:
		void setActive(bool active);
	
		// other public methods:
		void update(float dt, const std::vector<sf::Event>& events);
		void render(sf::RenderTarget& target);

	private:
		const sf::Font& font;
		
		std::string input;
		
		float char_size;

		sf::Color color;
		sf::Color textColor;
		sf::Color outlineColor;

		bool active;

		int id;

		bool change;

		sf::RectangleShape rect;
		
		sf::RectangleShape textCursor;

		sf::Text text;

		float stopwatch;

		// private methods:
		void updateTextAndCursorPositions();
	};
}
