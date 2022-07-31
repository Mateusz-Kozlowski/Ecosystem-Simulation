#include "BrainParameterInfo.h"

gui::BrainParameterInfo::BrainParameterInfo(
	const sf::Vector2f& size, 
	const sf::Color& bgColor,
	const sf::Font& font,
	const sf::VideoMode& resolution)
{
	initBg(size, bgColor);
	initText(font, resolution);
}

void gui::BrainParameterInfo::setSynapseInfo(const Blueberry::Synapse& synapse)
{
	std::string string("parameter type: synapse");

	string.append("\nweight: ");
	string.append(std::to_string(synapse.getWeight()));

	m_text.setString(string);
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

	m_text.setString(string);
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

void gui::BrainParameterInfo::initBg(
	const sf::Vector2f& size, 
	const sf::Color& bgColor)
{
	m_bg.setSize(size);
	m_bg.setFillColor(bgColor);
}

void gui::BrainParameterInfo::initText(
	const sf::Font& font, 
	const sf::VideoMode& resolution)
{
	m_text.setFont(font);
	m_text.setFillColor(sf::Color::White); // TODO: unhardcode
	m_text.setCharacterSize(gui::calcCharSize(16.0f, resolution));
	m_text.setPosition(m_bg.getPosition());
}
