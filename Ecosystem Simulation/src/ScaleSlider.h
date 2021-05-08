#pragma once

#include "Gui.h"

namespace gui
{
	class ScaleSlider
	{
	public:
		ScaleSlider(
			float posX, float posY,
			float textureScale,
			float maxValue, bool maxOnLeft,
			const std::string& axis_idle_path, const std::string& handle_idle_path,
			const std::string& axis_hover_path, const std::string& handle_hover_path,
			const std::string& axis_pressed_path, const std::string& handle_pressed_path
		);

		float getValue() const;
		float getMaxValue() const;

		void setValue(float value);
		void setMinimizeToZero(bool minimizeToZero);

		void update(sf::Vector2i mousePosWindow);
		void render(sf::RenderTarget& target);

	private:
		std::unordered_map<std::string, sf::Texture> axisTextures;
		std::unordered_map<std::string, sf::Texture> handleTextures;

		std::unordered_map<std::string, sf::Sprite> axes;
		std::unordered_map<std::string, sf::Sprite> handles;

		std::string state;

		bool maxOnLeft;
		bool minimizeToZero;

		float maxValue;
		float value;
	};
}
