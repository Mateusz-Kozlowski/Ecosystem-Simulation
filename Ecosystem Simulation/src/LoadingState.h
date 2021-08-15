#pragma once

#include "State.h"
#include "Button.h"
#include "TextBox.h"

class LoadingState : public State
{
public:
	LoadingState(StateData* stateData);
	
	virtual void update(float dt) override;
	virtual void render(sf::RenderTarget* target = nullptr) override;

	// mutators:
	
	virtual void freeze() override;

private:
	virtual void initKeybinds() override;
	void initBackground();
	void initContainer();
	void initFonts();
	void initTexts();
	void initTextBox();
	void initButtons();
	
	virtual void updateInput() override;
	void getUpdateFromButtons();

private:
	typedef std::unique_ptr<sf::Text> TextUniquePtr;
	typedef std::unique_ptr<gui::Button> BtnUniquePtr;

	sf::RectangleShape m_background;
	sf::RectangleShape m_container;

	sf::Font m_font;

	std::unordered_map<const char*, TextUniquePtr> m_texts;

	std::unique_ptr<gui::TextBox> m_textBox;

	std::unordered_map<std::string, BtnUniquePtr> m_buttons;
};
