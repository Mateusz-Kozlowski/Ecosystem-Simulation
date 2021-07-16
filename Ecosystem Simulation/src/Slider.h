#pragma once

#include "Gui.h"

namespace gui
{
	class Slider
	{
	public:
		// constructor:
		Slider(
			const sf::Vector2f& position,
			float textures_scale,
			const std::pair<float, float>& range,
			const std::pair<float, float>& not_snapping_to_edges_range,
			float default_value,
			const std::string& axis_idle_path, const std::string& handle_idle_path,
			const std::string& axis_hover_path, const std::string& handle_hover_path,
			const std::string& axis_pressed_path, const std::string& handle_pressed_path,
			const std::string& scale_function = "linear"
		);

		// accessors:
		float getCurrentValue() const;

		const sf::Vector2f& getPosition() const;

		// mutators:
		void setValue(float value);

		void setPosition(const sf::Vector2f& new_pos);

		// other public methods:
		void update(sf::Vector2i mousePosWindow);
		void render(sf::RenderTarget& target);

	private:
		std::unordered_map<std::string, sf::Texture> axisTextures;
		std::unordered_map<std::string, sf::Texture> handleTextures;

		std::unordered_map<std::string, sf::Sprite> axis;
		std::unordered_map<std::string, sf::Sprite> handle;

		std::string state;

		std::pair<float, float> range;
		std::pair<float, float> notSnappingToEdgesRange;

		float value;

		std::string scaleFunction;

		// initialization:
		void initTextures(
			const std::string& axis_idle_path, const std::string& handle_idle_path,
			const std::string& axis_hover_path, const std::string& handle_hover_path,
			const std::string& axis_pressed_path, const std::string& handle_pressed_path
		);
		void initSprites(const sf::Vector2f& position, float textures_scale);

		// private utilities:
		float inverseScaleFunction(float y);
		float scaleFunctionValue(float x);

		void updateCurrentValue();
		void snapToEdges();
	};
}
