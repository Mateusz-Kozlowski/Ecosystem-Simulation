#pragma once

#include "State.h"
#include "Button.h"
#include "TextBox.h"

class EcosystemCreatorState : public State
{
public:
	EcosystemCreatorState(StateData* stateData);
	
	virtual void update(float dt) override;
	virtual void render(sf::RenderTarget* target = nullptr) override;

	// mutators:
	
	virtual void freeze() override;

private:
	virtual void initKeybinds() override;
	void initBackground();
	void initFont();
	void initGui();
	void initButtons();
	void initTextBoxes();
	void initTexts();

	void addText(
		const std::string& str,
		const std::string& hashKey,
		const sf::Font& font,
		unsigned charSize,
		const sf::Color& color,
		const sf::Vector2f& position
	);

	void loadEcosystemTemplate(const std::string& ecosystemName);

	static std::string removeFloatTrailingZeros(const std::string& string);

	void createEcosystem();

	virtual void updateInput() override;

	void updateGui(float dt);

	void getUpdatesFromGui();

	void renderGui(sf::RenderTarget& target);

private:
	typedef std::unique_ptr<sf::Text> TextUniquePtr;
	typedef std::unique_ptr<gui::Button> BtnUniquePtr;
	typedef std::unique_ptr<gui::TextBox> TextBoxUniquePtr;

	sf::RectangleShape m_background;

	sf::Font m_font;
	std::unordered_map<std::string, TextUniquePtr> m_texts;

	std::unordered_map<std::string, BtnUniquePtr> m_buttons;
	std::unordered_map<std::string, TextBoxUniquePtr> m_textBoxes;
};
