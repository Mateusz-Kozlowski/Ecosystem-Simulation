#pragma once

#include "Gui.h"

namespace gui
{
	class ProgressBar
	{
	public:
		ProgressBar();
		ProgressBar(
			const std::pair<double, double>& valuesRange,
			bool correctOverRangeValues,
			double defaultValue,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& backgroundColor,
			const sf::Color& progressRectColor
		);

		void render(sf::RenderTarget& target) const;

		// accessors:

		const std::pair<double, double>& getValuesRange() const;

		bool overRangeValuesAreCorrected() const;

		double getCurrentValue() const;

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getSize() const;

		const sf::Color& getBackgroundColor() const;
		const sf::Color& getProgressRectColor() const;

		// mutators:

		void setValuesRange(const std::pair<double, double>& valuesRange);

		void setCorrectingOverRangeValues(
			bool correctOverRangeValues
		);

		void setValue(double value);
		void increaseValue(double valueIncrease);
		void decreaseValue(double valueDecrease);

		void setPosition(const sf::Vector2f& position);
		void setSize(const sf::Vector2f& size);

		void setBackgroundColor(const sf::Color& backgroundColor);
		void setProgressRectColor(const sf::Color& progressRectColor);

	private:
		// initialization:

		void initBackground(
			const sf::Vector2f& position,
			const sf::Color& backgroundColor
		);
		void initProgressRect(
			const sf::Vector2f& position, 
			const sf::Color& progressColor
		);
		
		// utils:

		void avoidOverRangeValue();

		void updateProgressRectSize();

		double getRangeLength();

	private:
		std::pair<double, double> m_valuesRange;
		
		bool m_overRangeValuesAreCorrected;

		double m_value;

		sf::RectangleShape m_background;
		sf::RectangleShape m_progressRect;
	};
}
