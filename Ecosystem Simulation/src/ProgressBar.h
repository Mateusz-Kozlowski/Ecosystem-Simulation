#pragma once

#include "Gui.h"

namespace gui
{
	class ProgressBar
	{
	public:
		ProgressBar();
		ProgressBar(
			const sf::Vector2f& valuesRange,
			bool correctOverRangeValues,
			float defaultValue,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& backgroundColor,
			const sf::Color& progressRectColor
		);

		void render(sf::RenderTarget& target) const;

		// accessors:

		const sf::Vector2f& getValuesRange() const;

		bool overRangeValuesAreCorrected() const;

		float getCurrentValue() const;

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getSize() const;

		const sf::Color& getBackgroundColor() const;
		const sf::Color& getProgressRectColor() const;

		// mutators:

		void setValuesRange(const sf::Vector2f& valuesRange);

		void setCorrectingOverRangeValues(
			bool correctOverRangeValues
		);

		void setValue(float value);
		void increaseValue(float valueIncrease);
		void decreaseValue(float valueDecrease);

		void setPosition(const sf::Vector2f& position);
		void setSize(const sf::Vector2f& size);

		void setBackgroundColor(const sf::Color& backgroundColor);
		void setProgressRectColor(const sf::Color& progressRectColor);

	private:
		void initBackground(
			const sf::Vector2f& position,
			const sf::Color& backgroundColor
		);
		void initProgressRect(
			const sf::Vector2f& position, 
			const sf::Color& progressColor
		);
		
		void avoidOverRangeValue();

		void updateProgressRectSize();

		float getRangeLength();

	private:
		sf::Vector2f m_valuesRange;

		bool m_overRangeValuesAreCorrected;

		float m_value;

		sf::RectangleShape m_background;
		sf::RectangleShape m_progressRect;
	};
}
