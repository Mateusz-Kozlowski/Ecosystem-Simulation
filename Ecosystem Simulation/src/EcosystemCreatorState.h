#pragma once

#include "State.h"
#include "Button.h"
#include "InputField.h"

class EcosystemCreatorState : public State
{
public:
	// constructor/destructor:
	EcosystemCreatorState(StateData* state_data);
	virtual ~EcosystemCreatorState();

	// mutators:
	virtual void freeze();

	// other public methods:
	virtual void update(float dt);
	virtual void render(sf::RenderTarget* target = nullptr);

private:
	sf::RectangleShape background;

	std::unordered_map<std::string, sf::Font> fonts;
	
	std::unordered_map<std::string, gui::Button*> buttons;

	std::unordered_map<std::string, gui::InputField*> inputFields;
	
	std::unordered_map<std::string, sf::Text*> texts;

	// initialization:
	virtual void initKeybinds();
	void initBackground();
	void initFonts();
	void initGui();
	void initButtons();
	void initInputFields();
	void initTexts();

	// private utilities:
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

	void loadEcosystem();

	// other private methods:
	virtual void updateInput();
	
	void updateGui(float dt);

	void getUpdatesFromGui();
	void getUpdatesFromEcosystemsTemplatesButtons();
	void getUpdatesFromButtons();

	void renderGui(sf::RenderTarget& target);
};
