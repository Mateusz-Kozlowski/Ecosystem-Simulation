#include "BrainParameterInfo.h"

gui::BrainParameterInfo::BrainParameterInfo(
	const sf::Color& textColor,
	const sf::Font& font,
	unsigned textCharsSize,
	const sf::Color& bgColor,
	const sf::Vector2f& pos)
{
	initText(pos, textColor, font, textCharsSize);
	initBg(bgColor);
}

void gui::BrainParameterInfo::setSynapseInfo(const Blueberry::Synapse& synapse)
{
	std::string string("parameter type: synapse");

	string.append("\nweight: ");
	string.append(std::to_string(synapse.getWeight()));

	m_text.setString(string);

	adjustBgSize();
}

void gui::BrainParameterInfo::setNeuronInfo(const Blueberry::Neuron& neuron)
{
	std::string string("parameter type: neuron");

	string.append("\nactivated val: ");
	string.append(std::to_string(neuron.getActVal()));

	string.append("\nval: ");
	string.append(std::to_string(neuron.getVal()));

	string.append("\nbias: ");
	string.append(std::to_string(neuron.getBias()));

	string.append("\nactivation function: ");
	string.append(neuron.getActFunc());
	
	if (neuron.getAdditionalInfo() != "")
	{
		string.append("\n");
		string.append(neuron.getAdditionalInfo());
	}

	m_text.setString(string);

	adjustBgSize();
}

void gui::BrainParameterInfo::setPos(const sf::Vector2f& pos)
{
	m_bg.setPosition(pos);
	m_text.setPosition(pos);
}

void gui::BrainParameterInfo::render(sf::RenderTarget& target) const
{
	target.draw(m_bg);
	target.draw(m_text);
}

// private methods:
// initialization:

void gui::BrainParameterInfo::initText(
	const sf::Vector2f& pos,
	const sf::Color& textColor,
	const sf::Font& font,
	unsigned textCharsSize)
{
	m_text.setPosition(pos);
	m_text.setFillColor(textColor);
	m_text.setFont(font);
	m_text.setCharacterSize(textCharsSize);
}

void gui::BrainParameterInfo::initBg(const sf::Color& bgColor)
{
	m_bg.setPosition(m_text.getPosition());
	m_bg.setFillColor(bgColor);
	adjustBgSize();
}

void gui::BrainParameterInfo::adjustBgSize()
{
	m_bg.setSize(
		sf::Vector2f(
			m_text.getLocalBounds().width,
			m_text.getLocalBounds().height + m_text.getCharacterSize() / 2.0f
		)
	);
}
