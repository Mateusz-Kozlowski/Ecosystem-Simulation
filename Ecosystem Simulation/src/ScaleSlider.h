#pragma once

#include "Gui.h"

namespace gui
{
	class ScaleSlider
	{
	public:
		// constructor:
		ScaleSlider(
			const sf::Vector2f& pos,
			float textures_scale,
			const std::pair<float, float>& range,
			float default_value,
			const std::string& axis_idle_path, const std::string& handle_idle_path,
			const std::string& axis_hover_path, const std::string& handle_hover_path,
			const std::string& axis_pressed_path, const std::string& handle_pressed_path
		);

		// accessors:
		float getCurrentValue() const;

		const sf::Vector2f& getPosition() const;

		// mutators:
		void setValue(float value);
		
		void setMinimizeToZero(bool minimizeToZero);

		void setPosition(const sf::Vector2f& new_pos);

		// other public methods:
		void update(sf::Vector2i mousePosWindow);
		void render(sf::RenderTarget& target);

	private:
		std::unordered_map<std::string, sf::Texture> axisTextures;
		std::unordered_map<std::string, sf::Texture> handleTextures;

		std::unordered_map<std::string, sf::Sprite> axes;
		std::unordered_map<std::string, sf::Sprite> handles;

		std::string state;

		std::pair<float, float> range;
		bool minimizeToZero;
		float value;
	};
}
