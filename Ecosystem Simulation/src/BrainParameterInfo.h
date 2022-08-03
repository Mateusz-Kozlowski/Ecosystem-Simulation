#pragma once

#include "gui.h"
#include "Synapse.h"
#include "Neuron.h"

namespace gui
{
	class BrainParameterInfo
	{
	public:
		BrainParameterInfo(
			const sf::Color& textColor,
			const sf::Font& font,
			unsigned textCharsSize,
			const sf::Color& bgColor,
			const sf::Vector2f& pos = sf::Vector2f(0.0f, 0.0f)
		);

		void setSynapseInfo(const Blueberry::Synapse& synapse);
		void setNeuronInfo(const Blueberry::Neuron& neuron);

		void setPos(const sf::Vector2f& pos);

		void render(sf::RenderTarget& target) const;

	private:
		// initialization:

		void initText(
			const sf::Vector2f& pos,
			const sf::Color& textColor,
			const sf::Font& font,
			unsigned textCharsSize
		);
		void initBg(const sf::Color& bgColor);

		void adjustBgSize();

	private:
		sf::RectangleShape m_bg;

		sf::Text m_text;
	};
}
