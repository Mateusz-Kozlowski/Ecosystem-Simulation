#include "pch.h"
#include "BrainPreview.h"

gui::BrainPreview::BrainPreview(
	const Blueberry::Brain& brain, 
	const sf::Vector2f& position, 
	const sf::Vector2f& size, 
	const sf::Color& backgroundColor)
	: m_brain(brain)
	, m_background(size)
	, m_neurons()
	, m_synapses()
{
	initBackground(position, backgroundColor);
	initNeurons();
	initSynapses();
}

void gui::BrainPreview::update()
{
	handleNewNeurons();
	handleNewSynapses();
	setNeuronsColors();
	setSynapsesColors();
}

void gui::BrainPreview::render(sf::RenderTarget& target) const
{
	target.draw(m_background);
	renderSynapses(target);
	renderNeurons(target);
}

// accessors:

const Blueberry::Brain& gui::BrainPreview::getBrain() const
{
	return m_brain;
}

const sf::Vector2f& gui::BrainPreview::getPosition() const
{
	return m_background.getPosition();
}

const sf::Vector2f& gui::BrainPreview::getSize() const
{
	return m_background.getSize();
}

const sf::Color& gui::BrainPreview::getBackgroundColor() const
{
	return m_background.getFillColor();
}

// mutators:

void gui::BrainPreview::setPosition(const sf::Vector2f& newPosition)
{
	move(
		newPosition.x - m_background.getPosition().x,
		newPosition.y - m_background.getPosition().y
	);
}

void gui::BrainPreview::setPosition(float x, float y)
{
	move(
		x - m_background.getPosition().x,
		y - m_background.getPosition().y
	);
}

void gui::BrainPreview::move(const sf::Vector2f& offset)
{
	move(offset.x, offset.y);
}

void gui::BrainPreview::move(float xOffset, float yOffset)
{
	m_background.move(xOffset, yOffset);
	moveNeurons(xOffset, yOffset);
	setSynapsesPos();
}

void gui::BrainPreview::setSize(const sf::Vector2f& newSize)
{
	scale(
		newSize.x / m_background.getSize().x,
		newSize.y / m_background.getSize().y
	);
}

void gui::BrainPreview::setSize(float x, float y)
{
	scale(
		x / m_background.getSize().x,
		y / m_background.getSize().y
	);
}

void gui::BrainPreview::scale(const sf::Vector2f& scaleFactors)
{
	scale(scaleFactors.x, scaleFactors.y);
}

void gui::BrainPreview::scale(float xScaleFactor, float yScaleFactor) 
{
	m_background.scale(xScaleFactor, yScaleFactor);

	setNeuronsSizes();
	
	setInputNeuronsPos();
	scaleHiddenNeuronsPos(xScaleFactor, yScaleFactor);
	setOutputNeuronsPos();

	setSynapsesPos();
}

void gui::BrainPreview::setBackgroundColor(const sf::Color& color)
{
	m_background.setFillColor(color);
}

// private methods:

// initialization:

void gui::BrainPreview::initBackground(
	const sf::Vector2f& position, 
	const sf::Color& backgroundColor)
{
	m_background.setPosition(position);
	m_background.setFillColor(backgroundColor);
}

void gui::BrainPreview::initNeurons()
{
	m_neurons.resize(
		m_brain.getEnabledNeuronsCount() 
		+ m_brain.getDisabledNeuronsCount()
	);

	setNeuronsSizes();

	setInputNeuronsPos();
	setRandomHiddenNeuronsPos();
	setOutputNeuronsPos();
	
	setNeuronsColors();
}

void gui::BrainPreview::initSynapses()
{
	const unsigned totalSynapsesCount = m_brain.getEnabledSynapsesCount()
		+ m_brain.getDisabledSynapsesCount();

	m_synapses.resize(
		totalSynapsesCount, 
		std::vector<sf::Vertex>(2)
	);
	
	setSynapsesPos();
	setSynapsesColors();
}

// utils:

void gui::BrainPreview::setNeuronsSizes()
{
	const float neuronRadius = calcNeuronsRadius();

	for (auto& neuron : m_neurons)
	{
		neuron.setRadius(neuronRadius);
	}
}

float gui::BrainPreview::calcNeuronsRadius() const
{
	// notice that there aren't any hidden layers in Blueberry::Brain:

	const unsigned theBiggestLayersSize = std::max(
		m_brain.getInputSize(),
		m_brain.getOutputSize()
	);
			
	return m_background.getSize().y / (1U + 3U * theBiggestLayersSize);
}

void gui::BrainPreview::setInputNeuronsPos()
{
	const sf::Vector2f& bgPos = m_background.getPosition();
	const float topMargin = calcInputLayerTopMargin();
	const float neuronsRadius = calcNeuronsRadius();

	for (int i = 0; i < m_brain.getInputSize(); i++)
	{
		assert(!m_brain.getNeurons()[i].disabled());

		m_neurons[i].setPosition(
			bgPos.x + neuronsRadius,
			bgPos.y + topMargin + 3 * i * neuronsRadius
		);
	}
}

float gui::BrainPreview::calcInputLayerTopMargin() const
{
	const float radius = calcNeuronsRadius();
	float bgHeightWithoutMargins = radius * (3 * m_brain.getInputSize() - 1);

	return (m_background.getSize().y - bgHeightWithoutMargins) / 2.0f;
}

void gui::BrainPreview::setRandomHiddenNeuronsPos()
{
	const unsigned inputSize = m_brain.getInputSize();
	const unsigned outputSize = m_brain.getOutputSize();

	for (int i = inputSize + outputSize; i < m_neurons.size(); i++)
	{
		setRandomPos(m_neurons[i]);
	}
}

void gui::BrainPreview::setOutputNeuronsPos()
{
	const unsigned inputSize = m_brain.getInputSize();
	const unsigned outputSize = m_brain.getOutputSize();
	
	const sf::FloatRect& bgBounds = m_background.getGlobalBounds();
	const float topMargin = calcOutputLayerTopMargin();
	const float neuronsRadius = calcNeuronsRadius();

	for (int i = inputSize; i < inputSize + outputSize; i++)
	{
		assert(!m_brain.getNeurons()[i].disabled());

		m_neurons[i].setPosition(
			bgBounds.left + bgBounds.width - 3 * neuronsRadius,
			bgBounds.top + topMargin + 3 * (i - inputSize) * neuronsRadius
		);
	}
}

float gui::BrainPreview::calcOutputLayerTopMargin() const
{
	const float radius = calcNeuronsRadius();
	float bgHeightWithoutMargins = radius * (3 * m_brain.getOutputSize() - 1);

	return (m_background.getSize().y - bgHeightWithoutMargins) / 2.0f;
}

void gui::BrainPreview::setNeuronsColors()
{
	const Blueberry::Scalar theBiggestAbsActVal = getTheBiggestAbsActVal();

	for (int i = 0; i < m_neurons.size(); i++)
	{
		setColor(
			m_neurons[i], 
			m_brain.getNeurons()[i], 
			theBiggestAbsActVal
		);
	}
}

Blueberry::Scalar gui::BrainPreview::getTheBiggestAbsActVal() const
{
	return std::max(abs(getTheBiggestActVal()), abs(getTheSmallestActVal()));
}

Blueberry::Scalar gui::BrainPreview::getTheBiggestActVal() const
{
	Blueberry::Scalar theBiggestActVal = -INFINITY;

	for (const auto& neuron : m_brain.getNeurons())
	{
		if (!neuron.disabled())
		{
			theBiggestActVal = std::max(
				theBiggestActVal, 
				neuron.getActVal()
			);
		}
	}
	
	return theBiggestActVal;
}

Blueberry::Scalar gui::BrainPreview::getTheSmallestActVal() const
{
	Blueberry::Scalar theSmallestActVal = INFINITY;

	for (const auto& neuron : m_brain.getNeurons())
	{
		if (!neuron.disabled())
		{
			theSmallestActVal = std::min(
				theSmallestActVal,
				neuron.getActVal()
			);
		}
	}

	return theSmallestActVal;
}

void gui::BrainPreview::setColor(
	sf::CircleShape& neuronCircle,
	const Blueberry::Neuron& neuron,
	const Blueberry::Scalar& theBiggestAbsActVal)
{
	if (neuron.disabled())
	{
		neuronCircle.setFillColor(sf::Color::Transparent);
	}
	else
	{
		const Blueberry::Scalar& actVal = neuron.getActVal();

		if (actVal > 0.0)
		{
			neuronCircle.setFillColor(
				sf::Color(
					0,
					0,
					255 * actVal / theBiggestAbsActVal
				)
			);
		}
		else
		{
			neuronCircle.setFillColor(
				sf::Color(
					-255 * actVal / theBiggestAbsActVal,
					0,
					0
				)
			);
		}
	}
}

void gui::BrainPreview::setSynapsesPos()
{
	const unsigned totalSynapsesCount = m_brain.getEnabledSynapsesCount()
		+ m_brain.getDisabledSynapsesCount();

	assert(m_synapses.size() == totalSynapsesCount);

	for (int i = 0; i < m_synapses.size(); i++)
	{
		setPos(m_synapses[i], m_brain.getSynapses()[i]);
	}
}

void gui::BrainPreview::setPos(
	std::vector<sf::Vertex>& synapseVertices, 
	const Blueberry::Synapse& synapse)
{
	if (!synapse.disabled())
	{
		const float neuronRadius = calcNeuronsRadius();

		synapseVertices[0].position = sf::Vector2f(
			m_neurons[synapse.getSrc()].getPosition().x + neuronRadius,
			m_neurons[synapse.getSrc()].getPosition().y + neuronRadius
		);

		synapseVertices[1].position = sf::Vector2f(
			m_neurons[synapse.getDst()].getPosition().x + neuronRadius,
			m_neurons[synapse.getDst()].getPosition().y + neuronRadius
		);
	}
}

void gui::BrainPreview::setSynapsesColors()
{
	const unsigned totalSynapsesCount = m_brain.getEnabledSynapsesCount()
		+ m_brain.getDisabledSynapsesCount();

	assert(m_synapses.size() == totalSynapsesCount);
	
	const Blueberry::Scalar theBiggestAbsWeight = getTheBiggestAbsWeight();

	for (int i = 0; i < m_synapses.size(); i++)
	{
		setColor(
			m_synapses[i], 
			m_brain.getSynapses()[i], 
			theBiggestAbsWeight
		);
	}
}

void gui::BrainPreview::setColor(
	std::vector<sf::Vertex>& synapseVertices, 
	const Blueberry::Synapse& synapse,
	const Blueberry::Scalar& theBiggestAbsWeight)
{
	if (synapse.disabled())
	{
		const sf::Color color = sf::Color::Transparent;

		synapseVertices[0].color = color;
		synapseVertices[1].color = color;

		return;
	}
	
	const Blueberry::Scalar& weight = synapse.getWeight();

	if (weight > 0.0)
	{
		const sf::Color color = sf::Color(
			0,
			0,
			255 * weight / theBiggestAbsWeight
		);

		synapseVertices[0].color = color;
		synapseVertices[1].color = color;
	}
	else
	{
		const sf::Color color = sf::Color(
			-255 * weight / theBiggestAbsWeight,
			0,
			0
		);

		synapseVertices[0].color = color;
		synapseVertices[1].color = color;
	}
}

Blueberry::Scalar gui::BrainPreview::getTheBiggestAbsWeight() const
{
	return std::max(abs(getTheBiggestWeight()), abs(getTheSmallestWeight()));
}

Blueberry::Scalar gui::BrainPreview::getTheBiggestWeight() const
{
	Blueberry::Scalar theBiggestWeight = -INFINITY;

	for (const auto& synapse : m_brain.getSynapses())
	{
		if (!synapse.disabled())
		{
			theBiggestWeight = std::max(
				theBiggestWeight,
				synapse.getWeight()
			);
		}
	}

	return theBiggestWeight;
}

Blueberry::Scalar gui::BrainPreview::getTheSmallestWeight() const
{
	Blueberry::Scalar theSmallestWeight = INFINITY;

	for (const auto& synapse : m_brain.getSynapses())
	{
		if (!synapse.disabled())
		{
			theSmallestWeight = std::min(
				theSmallestWeight,
				synapse.getWeight()
			);
		}
	}

	return theSmallestWeight;
}

void gui::BrainPreview::handleNewNeurons()
{
	const unsigned totalNeuronsCount = m_brain.getEnabledNeuronsCount()
		+ m_brain.getDisabledNeuronsCount();

	if (m_neurons.size() == totalNeuronsCount) return;

	assert(m_neurons.size() < totalNeuronsCount);

	const unsigned previousNeuronsCount = m_neurons.size();

	m_neurons.resize(totalNeuronsCount);

	const float neuronRadius = calcNeuronsRadius();
	const Blueberry::Scalar theBiggestAbsActVal = getTheBiggestAbsActVal();

	for (int i = previousNeuronsCount; i < m_neurons.size(); i++)
	{
		m_neurons[i].setRadius(neuronRadius);

		setRandomPos(m_neurons[i]);
		
		setColor(
			m_neurons[i], 
			m_brain.getNeurons()[i], 
			theBiggestAbsActVal
		);
	}
}

void gui::BrainPreview::setRandomPos(sf::CircleShape& neuron)
{
	const sf::FloatRect bgBorders = m_background.getGlobalBounds();
	const float neuronRadius = calcNeuronsRadius();
	
	const std::pair<float, float> xRange(
		bgBorders.left + 5.0f * neuronRadius,
		bgBorders.left + bgBorders.width - 7.0f * neuronRadius
	);

	const std::pair<float, float> yRange(
		bgBorders.top + neuronRadius,
		bgBorders.top + bgBorders.height - 3.0f * neuronRadius
	);

	neuron.setPosition(
		Blueberry::RandomEngine::getScalarInRange(
			xRange.first, 
			xRange.second
		),
		Blueberry::RandomEngine::getScalarInRange(
			yRange.first, 
			yRange.second
		)
	);
}

void gui::BrainPreview::handleNewSynapses()
{
	const unsigned totalSynapsesCount = m_brain.getEnabledSynapsesCount()
		+ m_brain.getDisabledSynapsesCount();

	if (m_synapses.size() == totalSynapsesCount) return;

	assert(m_synapses.size() < totalSynapsesCount);

	const unsigned previousSynapsesCount = m_synapses.size();

	m_synapses.resize(totalSynapsesCount, std::vector<sf::Vertex>(2));

	const Blueberry::Scalar theBiggestAbsWeight = getTheBiggestAbsWeight();

	for (int i = previousSynapsesCount; i < m_synapses.size(); i++)
	{
		setPos(m_synapses[i], m_brain.getSynapses()[i]);
		setColor(
			m_synapses[i],
			m_brain.getSynapses()[i],
			theBiggestAbsWeight
		);
	}
}

void gui::BrainPreview::renderNeurons(sf::RenderTarget& target) const
{
	for (const auto& neuron : m_neurons)
	{
		target.draw(neuron);
	}
}

void gui::BrainPreview::renderSynapses(sf::RenderTarget& target) const
{
	for (const auto& synapse : m_synapses)
	{
		target.draw(&synapse[0], synapse.size(), sf::Lines);
	}
}

void gui::BrainPreview::moveNeurons(float xOffset, float yOffset)
{
	for (auto& neuron : m_neurons)
	{
		neuron.move(xOffset, yOffset);
	}
}

void gui::BrainPreview::scaleHiddenNeuronsPos(
	float xScaleFactor, 
	float yScaleFactor)
{
	const unsigned inputSize = m_brain.getInputSize();
	const unsigned outputSize = m_brain.getOutputSize();

	const sf::Vector2f& bgPos = m_background.getPosition();

	for (int i = inputSize + outputSize; i < m_neurons.size(); i++)
	{
		const sf::Vector2f& prevPos = m_neurons[i].getPosition();	
		const sf::Vector2f& distFromBgOrigin = prevPos - bgPos;

		m_neurons[i].setPosition(
			bgPos.x + xScaleFactor * distFromBgOrigin.x,
			bgPos.y + yScaleFactor * distFromBgOrigin.y
		);
	}
}
