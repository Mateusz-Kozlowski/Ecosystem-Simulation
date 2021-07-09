#pragma once

#include "Gui.h"

namespace gui
{
	class ProgressBar
	{
	public:
		// constructors:
		ProgressBar();
		ProgressBar(
			const sf::Vector2f& values_range,
			bool correct_over_range_values,
			float default_value,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& background_color,
			const sf::Color& progress_rect_color
		);

		// public methods:
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
		void setValuesRange(const sf::Vector2f& values_range);

		void setCorrectingOverRangeValues(bool correct_over_range_values);

		void setValue(float value);

		void increaseValue(float value_increase);

		void decreaseValue(float value_decrease);

		void setPosition(const sf::Vector2f& position);

		void setSize(const sf::Vector2f& size);

		void setBackgroundColor(const sf::Color& background_color);

		void setProgressRectColor(const sf::Color& progress_rect_color);

	private:
		sf::Vector2f valuesRange;

		bool mOverRangeValuesAreCorrected;

		float value;

		sf::RectangleShape background;
		sf::RectangleShape progressRect;

		// private methods:
		// initialization:
		void initBackground(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& background_color
		);
		void initProgressRect(const sf::Vector2f& position, const sf::Color& progress_color);

		// private utilities:	
		void avoidOverRangeValue();

		void updateProgressRectSize();

		float getRangeLength();
	};
}
