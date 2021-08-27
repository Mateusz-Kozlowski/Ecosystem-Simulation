#pragma once

#include "Gui.h"

namespace gui
{
	class Slider
	{
	public:
		Slider(
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
		
		void update(sf::Vector2i mousePosWindow);
		void render(sf::RenderTarget& target);

		// accessors:

		float getCurrentValue() const;

		const sf::Vector2f& getPosition() const;

		// mutators:
		
		void setValue(float value);

		void setPosition(const sf::Vector2f& newPos);

	private:
		// initialization:

		void initTextures(
			const std::string& axisIdlePath, 
			const std::string& handleIdlePath,
			const std::string& axisHoverPath, 
			const std::string& handleHoverPath,
			const std::string& axisPressedPath, 
			const std::string& handlePressedPath
		);
		void initSprites(
			const sf::Vector2f& position, 
			float texturesScale
		);
		
		// utils:

		float inverseScaleFunction(float y);
		float scaleFunctionValue(float x);

		void updateCurrentValue();
		void snapToEdges();

	private:
		std::unordered_map<std::string, sf::Texture> m_axisTextures;
		std::unordered_map<std::string, sf::Texture> m_handleTextures;

		std::unordered_map<std::string, sf::Sprite> m_axisSprites;
		std::unordered_map<std::string, sf::Sprite> m_handleSprites;

		std::string m_state;

		std::pair<float, float> m_range;
		std::pair<float, float> m_notSnappingToEdgesRange;

		float m_value;

		std::string m_scaleFunction;
	};
}
