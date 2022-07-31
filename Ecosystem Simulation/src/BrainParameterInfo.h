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
			const sf::Vector2f& size,
			const sf::Color& bgColor,
			const sf::Font& font,
			const sf::VideoMode& resolution
		);

		void setSynapseInfo(const Blueberry::Synapse& synapse);
		void setNeuronInfo(const Blueberry::Neuron& neuron);

		void setPos(const sf::Vector2f& pos);

		void render(sf::RenderTarget& target) const;

	private:
		// initialization:

		void initBg(
			const sf::Vector2f& size,
			const sf::Color& bgColor
		);
		void initText(
			const sf::Font& font,
			const sf::VideoMode& resolution
		);

	private:
		sf::RectangleShape m_bg;

		sf::Text m_text;
	};
}
