#pragma once

#include "Gui.h"

class PauseMenu
{
private:
	sf::VideoMode videoMode;
	sf::Font font;

	sf::RectangleShape background;
	sf::RectangleShape container;

	sf::Text title;

	std::unordered_map<std::string, gui::Button*> buttons;

public:
	PauseMenu(const sf::VideoMode& vm, const sf::Font& font);
	~PauseMenu();

	std::unordered_map<std::string, gui::Button*>& getButtons();

	bool isButtonClicked(std::string key);

	void addButton(
		std::string key,
		float posY,
		float width, float height,
		int charSize, std::string text,
		sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
		sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
		sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
		float outlineThickness = 1.f, short unsigned id = 0
	);

	void addText(float posY, unsigned charSize, std::string text, sf::Color textColor);

	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};
