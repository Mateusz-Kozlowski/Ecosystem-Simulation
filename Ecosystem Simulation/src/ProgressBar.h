#pragma once

#include "Gui.h"

namespace gui
{
	class IntProgressBar
	{
	public:
		IntProgressBar();
		IntProgressBar(
			const sf::Vector2i& valuesRange,
			int defaultValue,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& backgroundColor,
			const sf::Color& progressRectColor
		);

		void render(sf::RenderTarget& target) const;

		// accessors:

		const sf::Vector2i& getValuesRange() const;

		int getCurrentValue() const;

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getSize() const;

		const sf::Color& getBackgroundColor() const;
		const sf::Color& getProgressRectColor() const;

		// mutators:

		void setValuesRange(const sf::Vector2i& valuesRange);

		void setValue(int value);
		void increaseValue(int valueIncrease);
		void decreaseValue(int valueDecrease);

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
		
		void updateProgressRectSize();

		int getRangeLength();

	private:
		sf::Vector2i m_valuesRange;
		
		int m_value;

		sf::RectangleShape m_background;
		sf::RectangleShape m_progressRect;
	};
}
