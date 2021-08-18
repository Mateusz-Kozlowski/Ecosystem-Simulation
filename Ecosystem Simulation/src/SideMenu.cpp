#include "pch.h"
#include "SideMenu.h"

gui::SideMenu::SideMenu(
	const sf::Vector2f& position, 
	const sf::Vector2f& size,
	const sf::Color& backgroundColor)
	: m_background(size)
	, m_textureButtons()
	, m_buttons()
	, m_sliders()
	, m_texts()
{
	initBackground(position, backgroundColor);
}

void gui::SideMenu::update(
	const sf::Vector2i& mousePosWindow, 
	const std::vector<sf::Event>& events)
{
	for (auto& textureButton : m_textureButtons)
	{
		textureButton.second->update(mousePosWindow, events);
	}

	for (auto& button : m_buttons)
	{
		button.second->update(mousePosWindow);
	}

	for (auto& scaleSlider : m_sliders)
	{
		scaleSlider.second->update(mousePosWindow);
	}
}

void gui::SideMenu::render(sf::RenderTarget& target)
{
	target.draw(m_background);

	for (const auto& textureButton : m_textureButtons)
	{
		textureButton.second->render(target);
	}

	for (const auto& button : m_buttons)
	{
		button.second->render(target);
	}

	for (const auto& scaleSlider : m_sliders)
	{
		scaleSlider.second->render(target);
	}

	for (const auto& text : m_texts)
	{
		target.draw(text);
	}
}

// accessors:

const sf::Vector2f& gui::SideMenu::getPosition() const
{
	return m_background.getPosition();
}

const sf::Vector2f& gui::SideMenu::getSize() const
{
	return m_background.getSize();
}

const gui::SideMenu::ImgBtnsMap& gui::SideMenu::getImageButtons() const
{
	return m_textureButtons;
}

const gui::SideMenu::BtnsMap& gui::SideMenu::getButtons() const
{
	return m_buttons;
}

const gui::SideMenu::SlidersMap& gui::SideMenu::getSliders() const
{
	return m_sliders;
}

bool gui::SideMenu::hasImageButtonBeenClicked(const std::string& key)
{
	return m_textureButtons[key]->hasBeenClicked();
}

const sf::RectangleShape& gui::SideMenu::getBackground() const
{
	return m_background;
}

// mutators:

void gui::SideMenu::setPosition(const sf::Vector2f& newPos)
{
	sf::Vector2f oldPos = m_background.getPosition();
	
	m_background.setPosition(newPos);

	// texture buttons:
	for (auto& textureButton : m_textureButtons)
		textureButton.second->setPosition(
			sf::Vector2f(
				newPos.x - oldPos.x + textureButton.second->getPosition().x,
				newPos.y - oldPos.y + textureButton.second->getPosition().y
			)
		);

	// buttons:
	for (auto& button : m_buttons)
		button.second->setPosition(
			sf::Vector2f(
				newPos.x - oldPos.x + button.second->getPosition().x,
				newPos.y - oldPos.y + button.second->getPosition().y
			)
		);

	// scale sliders:
	for (auto& scaleSlider: m_sliders)
		scaleSlider.second->setPosition(
			sf::Vector2f(
				newPos.x - oldPos.x + scaleSlider.second->getPosition().x,
				newPos.y - oldPos.y + scaleSlider.second->getPosition().y
			)
		);

	// texts:
	for (auto& text : m_texts)
		text.setPosition(
			newPos.x - oldPos.x + text.getPosition().x,
			newPos.y - oldPos.y + text.getPosition().y
		);
}

void gui::SideMenu::addImageButton(
	const std::string& key,
	const std::vector<StringsPair>& texturesPathAndKeys,
	const std::string& keyOfDefaultTexture,
	const sf::Vector2f& position, 
	const sf::Vector2f& size,
	int id)
{
	m_textureButtons[key] = std::make_unique<gui::ImageButton>(
		texturesPathAndKeys,
		keyOfDefaultTexture,
		position,
		size
	);
}

void gui::SideMenu::addButton(
	const std::string& key, 
	const sf::Vector2f& position, 
	const sf::Vector2f& size, 
	int charSize, 
	const sf::Font& font, 
	const std::string& text,
	sf::Color idleColor, 
	sf::Color hoverColor, 
	sf::Color pressedColor,
	sf::Color outlineIdleColor, 
	sf::Color outlineHoverColor, 
	sf::Color outlinePressedColor,
	sf::Color textIdleColor, 
	sf::Color textHoverColor, 
	sf::Color textPressedColor,
	float outlineThickness, 
	short unsigned id)
{
	m_buttons[key] = std::make_unique<gui::Button>(
		position,
		size,
		font, text, charSize,
		idleColor, hoverColor, pressedColor,
		outlineIdleColor, outlineHoverColor, outlinePressedColor,
		textIdleColor, textHoverColor, textPressedColor,
		outlineThickness, id
	);
}

void gui::SideMenu::addSlider(
	const std::string& key,
	const sf::Vector2f& position, 
	float texturesScale, 
	const std::pair<float, float>& range,
	const std::pair<float, float>& notSnappingToEdgesRange,
	float defaultValue, 
	const std::string& axisIdlePath, 
	const std::string& handleIdlePath, 
	const std::string& axisHoverPath, 
	const std::string& handleHoverPath, 
	const std::string& axisPressedPath, 
	const std::string& handlePressedPath,
	const std::string& scaleFunction)
{
	m_sliders[key] = std::make_unique<gui::Slider>(
		position,
		texturesScale,
		range,
		notSnappingToEdgesRange,
		defaultValue,
		axisIdlePath, handleIdlePath,
		axisHoverPath, handleHoverPath,
		axisPressedPath, handlePressedPath,
		scaleFunction
	);
}

void gui::SideMenu::addCenteredText(
	float posY, 
	unsigned charSize, 
	const sf::Font& font,
	const std::string& text, 
	sf::Color color)
{
	m_texts.emplace_back();

	m_texts.back().setFont(font);
	m_texts.back().setString(text);
	m_texts.back().setCharacterSize(charSize);
	m_texts.back().setFillColor(color);

	const sf::Vector2f& bgPos = m_background.getPosition();
	const sf::Vector2f& bgSize = m_background.getSize();
	sf::FloatRect textBounds = m_texts.back().getLocalBounds();

	m_texts.back().setPosition(
		bgPos.x + bgSize.x / 2.f - textBounds.width / 2.f,
		posY
	);
}

void gui::SideMenu::setTextureOfImgBtn(
	const std::string& buttonKey, 
	const std::string& textureKey)
{
	m_textureButtons[buttonKey]->setTexture(textureKey);
}

// private methods:
 
void gui::SideMenu::initBackground(
	const sf::Vector2f& position,
	const sf::Color& color)
{
	m_background.setPosition(position);
	m_background.setFillColor(color);
}
