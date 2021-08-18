#pragma once

#include "ImageButton.h"
#include "Button.h"
#include "Slider.h"

namespace gui
{
	class SideMenu
	{
	private:
		typedef std::unique_ptr<gui::ImageButton> ImgBtnUniquePtr;
		typedef std::unique_ptr<gui::Button> BtnUniquePtr;
		typedef std::unique_ptr<gui::Slider> SliderUniquePtr;
		typedef std::pair<std::string, std::string> StringsPair;

		typedef std::unordered_map<std::string, ImgBtnUniquePtr> ImgBtnsMap;
		typedef std::unordered_map<std::string, BtnUniquePtr> BtnsMap;
		typedef std::unordered_map<std::string, SliderUniquePtr> SlidersMap;

	public:
		SideMenu(
			const sf::Vector2f& position, 
			const sf::Vector2f& size,
			const sf::Color& backgroundColor
		);
		
		void update(
			const sf::Vector2i& mousePosWindow, 
			const std::vector<sf::Event>& events
		);
		void render(sf::RenderTarget& target);
		
		// accessors:

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getSize() const;

		const ImgBtnsMap& getImageButtons() const;
		const BtnsMap& getButtons() const;
		const SlidersMap& getSliders() const;

		bool hasImageButtonBeenClicked(const std::string& key);

		const sf::RectangleShape& getBackground() const;
			
		// mutators:

		void setPosition(const sf::Vector2f& newPos);

		void addImageButton(
			const std::string& key,
			const std::vector<StringsPair>& texturesPathAndKeys,
			const std::string& keyOfDefaultTexture,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			int id = 0
		);

		void setTextureOfImgBtn(
			const std::string& buttonKey,
			const std::string& textureKey
		);

		void addButton(
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
			float outlineThickness = 1.f, 
			short unsigned id = 0
		);

		void addSlider(
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
			const std::string& scaleFunction = "linear"
		);

		void addCenteredText(
			float posY,
			unsigned charSize,
			const sf::Font& font,
			const std::string& text,
			sf::Color color
		);

	private:
		void initBackground(
			const sf::Vector2f& position,
			const sf::Color& color
		);

	private:
		sf::RectangleShape m_background;

		ImgBtnsMap m_textureButtons;
		BtnsMap m_buttons;
		SlidersMap m_sliders;

		std::vector<sf::Text> m_texts;
	};
}
