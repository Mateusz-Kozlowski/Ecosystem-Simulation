#pragma once

#include "State.h"
#include "Button.h"
#include "TextBox.h"

class EcosystemCreatorState : public State
{
public:
	EcosystemCreatorState(StateData* state_data);

	// public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

	// mutators:
	virtual void freeze();

private:
	sf::RectangleShape background;

	std::unordered_map<std::string, sf::Font> fonts;

	std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

	std::unordered_map<std::string, std::unique_ptr<gui::TextBox>> textBoxes;

	std::unordered_map<std::string, std::unique_ptr<sf::Text>> texts;

	// private methods:
	// initialization:
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initGui();
	void initButtons();
	void initTextBoxes();
	void initTexts();

	// other private methods:
	void addText(
		const std::string& str,
		const std::string& hash_key,
		const sf::Font& font,
		unsigned char_size,
		const sf::Color& color,
		const sf::Vector2f& position
	);

	void loadEcosystemTemplate(const std::string& ecosystem_template_button_key);

	static std::string removeFloatTrailingZeros(const std::string& string);

	void createEcosystem();
	
	virtual void updateInput();

	void updateGui(float dt);

	void getUpdatesFromGui();

	void renderGui(sf::RenderTarget& target);
};
