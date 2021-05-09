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
			float outline_thickness, 
			int id = 0
		);

		// accessors:
		int getId() const;
		const std::string& getInput() const;
	
		// other public methods:
		void update(float dt, const std::vector<sf::Event>& events);
		void render(sf::RenderTarget& target);

	private:
		float posX, posY;
		float width, height;

		const sf::Font& font;
		std::string input;
		float charSize;

		sf::Color color, textColor, outlineColor;

		float outlineThickness;

		int id;

		bool change;

		sf::RectangleShape outline;
		sf::RectangleShape field;
		sf::RectangleShape textCursor;

		sf::Text text;

		float stopwatch;

		// private methods:
		void updateTextAndCursorPositions();
	};
}
