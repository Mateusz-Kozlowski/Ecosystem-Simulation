#pragma once

#include "TextureButton.h"
#include "Button.h"
#include "ScaleSlider.h"

namespace gui
{
	class SideMenu
	{
	public:
		// constructor/destructor:
		SideMenu(
			sf::Font& font, 
			const sf::Vector2f& pos, 
			const sf::Vector2f& size,
			const sf::Color& background_color
		);
		~SideMenu();

		// accessors:
		const sf::Vector2f& getPos() const;

		const sf::Vector2f& getSize() const;

		const std::unordered_map<std::string, gui::TextureButton*>& getTextureButtons() const;
		const std::unordered_map<std::string, gui::Button*>& getButtons() const;
		const std::unordered_map<std::string, gui::ScaleSlider*>& getScaleSliders() const;

		bool hasTextureButtonBeenClicked(const std::string& key);
			
		// mutators:
		void setPosition(const sf::Vector2f& new_pos);

		void addTextureButton(
			const std::string& key,
			const std::vector<std::pair<std::string, std::string>>& textures_path_and_keys,
			const std::string& key_of_default_texture,
			float pos_x, float pos_y,
			float width, float height,
			int id = 0
		);

		void setTextureOfTextureButton(
			const std::string& button_key,
			const std::string& texture_key
		);

		void addButton(
			const std::string& key,
			const sf::Vector2f& pos,
			float width, float height,
			int charSize, std::string text,
			sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
			sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
			sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
			float outlineThickness = 1.f, short unsigned id = 0
		);

		void addScaleSlider(
			const std::string& key,
			float posX, float posY,
			float textures_scale,
			const std::pair<float, float>& range,
			float default_value,
			const std::string& axis_idle_path, const std::string& handle_idle_path,
			const std::string& axis_hover_path, const std::string& handle_hover_path,
			const std::string& axis_pressed_path, const std::string& handle_pressed_path
		);

		//void addText(float posY, unsigned charSize, std::string text, sf::Color textColor);

		// other public methods:
		void update(const sf::Vector2i& mousePosWindow, const std::vector<sf::Event>& events);
		void render(sf::RenderTarget& target);

	private:
		sf::Font& font;

		sf::RectangleShape background;

		std::unordered_map<std::string, gui::TextureButton*> textureButtons;
		std::unordered_map<std::string, gui::Button*> buttons;
		std::unordered_map<std::string, gui::ScaleSlider*> scaleSliders;

		// initialization:
		void initBackground(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
	};
}
