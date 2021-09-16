#include "pch.h"
#include "NeuralNetPreview.h"

gui::NeuralNetPreview::NeuralNetPreview(
	const Blueberry::Brain& brain,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& backgroundColor)
	: m_brain(&brain)
	, m_background(size)
	, m_neurons()
	, m_synapses()
{
	initBackground(position, backgroundColor);
	initNeurons();
	initSynapses();
}

void gui::NeuralNetPreview::update()
{
	updateNeurons();
}

void gui::NeuralNetPreview::render(sf::RenderTarget& target) const
{
	target.draw(m_background);

	renderNeurons(target);
	renderSynapses(target);
}

// accessors:

const Blueberry::Brain& gui::NeuralNetPreview::getBrain() const
{
	return *m_brain;
}

const sf::Vector2f& gui::NeuralNetPreview::getPosition() const
{
	return m_background.getPosition();
}

const sf::Vector2f& gui::NeuralNetPreview::getSize() const
{
	return m_background.getSize();
}

const sf::Color& gui::NeuralNetPreview::getBackgroundColor() const
{
	return m_background.getFillColor();
}

// mutators:

void gui::NeuralNetPreview::setPosition(const sf::Vector2f& position)
{
	m_background.setPosition(position);

	setNeuronsPositions();
	setSynapsesPos();
}

void gui::NeuralNetPreview::setPosition(float x, float y)
{
	m_background.setPosition(x, y);

	setNeuronsPositions();
	setSynapsesPos();
}

void gui::NeuralNetPreview::setSize(const sf::Vector2f& size)
{
	m_background.setSize(size);

	setNeuronsPositions();
	setNeuronsSizes();
	setSynapsesPos();
}

void gui::NeuralNetPreview::setBackgroundColor(const sf::Color& color)
{
	m_background.setFillColor(color);
}

// private methods:

void gui::NeuralNetPreview::initBackground(
	const sf::Vector2f& previewPosition,
	const sf::Color& backgroundColor)
{
	m_background.setPosition(previewPosition);
	m_background.setFillColor(backgroundColor);
}

void gui::NeuralNetPreview::initNeurons()
{
	initNeuronsVector();
	setNeuronsPositions();
	setNeuronsSizes();
	setNeuronsColors();
}

void gui::NeuralNetPreview::initNeuronsVector()
{
	//unsigned hiddenLayersCount = m_brain->getHiddenLayers().m_size();
	//
	//m_neurons.resize(hiddenLayersCount + 2U);
	//
	//// resize input layer:
	//m_neurons[0].resize(
	//	m_brain->getInputLayer()->getSize(),
	//	sf::CircleShape(0.0f, neurons_shapes_point_count)
	//);
	//
	//// resize hidden layers:
	//for (int i = 0; i < hiddenLayersCount; i++)
	//{
	//	m_neurons[i + 1].resize(
	//		m_brain->getHiddenLayers()[i]->getNeuronsCount(),
	//		sf::CircleShape(0.0f, neurons_shapes_point_count)
	//	);
	//}
	//
	//// resize output layer:
	//m_neurons.back().resize(
	//	m_brain->getOutputLayer()->getNeuronsCount(),
	//	sf::CircleShape(0.0f, neurons_shapes_point_count)
	//);
}

void gui::NeuralNetPreview::initSynapses()
{
	initSynapsesVector();
	setSynapsesPos();
	setSynapsesColors();
}

void gui::NeuralNetPreview::initSynapsesVector()
{
	//auto inputLayer = m_brain->inputLayer();
	//auto hiddenLayers = m_brain->getHiddenLayers();
	//unsigned hiddenLayersCount = hiddenLayers.m_size();
	//auto outputLayer = m_brain->getOutputLayer();
	//
	//m_synapses.resize(hiddenLayersCount + 1U);
	//
	//// resize each matrix in m_synapses vector 
	//// (make m_synapses a 2D structure):
	//for (int i = 0; i < hiddenLayersCount; i++)
	//{
	//	m_synapses[i].resize(hiddenLayers[i]->getNeuronsCount());
	//}	
	//
	//m_synapses.back().resize(outputLayer->getNeuronsCount());
	//
	//// make m_synapses a 3D structure:
	//if (hiddenLayersCount > 0)
	//{
	//	for (int i = 0; i < hiddenLayers[0]->getNeuronsCount(); i++)
	//	{
	//		m_synapses[0][i].resize(inputLayer->getSize());
	//	}
	//
	//	for (int i = 0; i < hiddenLayersCount - 1; i++)
	//	{
	//		for (int j = 0; j < hiddenLayers[i + 1]->getNeuronsCount(); j++)
	//		{
	//			m_synapses[i + 1][j].resize(
	//				hiddenLayers[i]->getNeuronsCount()
	//			);
	//		}
	//	}
	//		
	//	for (int i = 0; i < outputLayer->getNeuronsCount(); i++)
	//	{
	//		m_synapses.back()[i].resize(
	//			hiddenLayers.back()->getNeuronsCount()
	//		);
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < outputLayer->getNeuronsCount(); i++)
	//	{
	//		m_synapses.back()[i].resize(inputLayer->output().m_size());
	//	}
	//}
	//
	//// each synapse consists of 2 vertices:
	//for (auto& matrix : m_synapses)
	//{
	//	for (auto& vector : matrix)
	//	{
	//		for (auto& synapse : vector)
	//		{
	//			synapse.resize(2);
	//		}
	//	}
	//}
}

void gui::NeuralNetPreview::setNeuronsPositions()
{
	for (int i = 0; i < m_neurons.size(); i++)
	{
		for (int j = 0; j < m_neurons[i].size(); j++)
		{
			m_neurons[i][j].setPosition(calcNeuronPosition(i, j));
		}
	}

}

sf::Vector2f gui::NeuralNetPreview::calcNeuronPosition(
	unsigned index1, 
	unsigned index2) const
{
	sf::Vector2f result;

	float gapBetweenLayers = calcGapBetweenLayers();
	float topMargin = calcTopMargin(index1);
	float diameter = 2.f * calcNeuronsRadius();

	result.x = m_background.getPosition().x 
			   + diameter 
			   + index1 * (diameter + gapBetweenLayers);
	
	result.y = m_background.getPosition().y 
			   + topMargin 
			   + 2U * index2 * diameter;

	return result;
}

float gui::NeuralNetPreview::calcGapBetweenLayers() const
{
	// TODO: after implementing a new version of Blueberry
	// TODO: uncomment and rewrite this!:
	
	//unsigned hiddenLayersCount = m_brain->getHiddenLayers().m_size();
	//float diameter = 2.f * calcNeuronsRadius();
	//
	//return (m_background.getSize().x - (4U + hiddenLayersCount) * diameter) 
	//	   / (hiddenLayersCount + 1U);
	
	return 0.0f;
}

float gui::NeuralNetPreview::calcNeuronsRadius() const
{
	float diameter = m_background.getSize().y 
					 / (2U * getTheBiggestLayerSize() + 1U);

	return diameter / 2.f;
}

float gui::NeuralNetPreview::calcTopMargin(unsigned index1) const
{
	float diameter = 2.f * calcNeuronsRadius();

	float result = m_background.getSize().y
				   - (2 * m_neurons[index1].size() - 1) * diameter;

	return result / 2.f;
}

unsigned gui::NeuralNetPreview::getTheBiggestLayerSize() const
{
	// TODO: after implementing a new version of Blueberry
	// TODO: uncomment and rewrite this!:
	
	//// input layer:
	//unsigned theBiggestLayerSize = m_brain->getInputLayer()->getSize();
	//
	//// hidden layers:
	//for (const auto& hiddenLayer : m_brain->getHiddenLayers())
	//{
	//	theBiggestLayerSize = std::max(
	//		getTheBiggestLayerSize, 
	//		hiddenLayer->getNeuronsCount()
	//	);
	//}
	//// output layer:
	//theBiggestLayerSize = std::max(
	//	getTheBiggestLayerSize, 
	//	m_brain->getOutputLayer()->getNeuronsCount()
	//);
	//
	//return getTheBiggestLayerSize;
	
	return 0U;
}

void gui::NeuralNetPreview::setNeuronsSizes()
{
	unsigned theBiggestLayerSize = getTheBiggestLayerSize();
	float radius = calcNeuronsRadius();

	for (auto& it1 : m_neurons)
	{
		for (auto& it2 : it1)
		{
			it2.setRadius(radius);
		}
	}
}

void gui::NeuralNetPreview::setNeuronsColors()
{
	setInputNeuronsColors();
	setHiddenNeuronsColors();
	setOutputNeuronsColors();
}

void gui::NeuralNetPreview::setInputNeuronsColors()
{
	//typedef Blueberry::Scalar Scalar;
	//
	//Scalar theBiggestActVal = getTheBiggestActivatedValue(0U);
	//Scalar theSmallestActVal = getTheSmallestActivatedValue(0U);
	//
	//Scalar theBiggestAbsActVal = std::max(
	//	abs(theBiggestActVal), 
	//	abs(theSmallestActVal)
	//);
	//
	//for (int i = 0; i < m_neurons[0].m_size(); i++)
	//{
	//	Scalar neuronActVal = m_brain->getInputLayer()->output()[i];
	//
	//	if (neuronActVal > 0.0)
	//	{
	//		m_neurons[0][i].setFillColor(
	//			sf::Color(
	//				0, 
	//				0, 
	//				255 * neuronActVal / theBiggestAbsActVal, 
	//				255
	//			)
	//		);
	//	}
	//	else
	//	{
	//		m_neurons[0][i].setFillColor(
	//			sf::Color(
	//				-255 * neuronActVal / theBiggestAbsActVal, 
	//				0, 
	//				0, 
	//				255
	//			)
	//		);
	//	}
	//}
}

void gui::NeuralNetPreview::setHiddenNeuronsColors()
{
	//typedef Blueberry::Scalar Scalar;
	//
	//auto hiddenLayers = m_brain->getHiddenLayers();
	//
	//for (int i = 0; i < hiddenLayers.m_size(); i++)
	//{
	//	Scalar theBiggestActVal = getTheBiggestActivatedValue(i + 1);
	//	Scalar theSmallestActVal = getTheSmallestActivatedValue(i + 1);
	//
	//	Scalar theBiggestAbsActVal = std::max(
	//		abs(theBiggestActVal), 
	//		abs(theSmallestActVal)
	//	);
	//
	//	for (int j = 0; j < m_neurons[i + 1].m_size(); j++)
	//	{
	//		Scalar actVal = hiddenLayers[i]->getNeurons()[j]->getActivatedValue();
	//
	//		if (actVal > 0.0)
	//		{
	//			m_neurons[i + 1][j].setFillColor(
	//				sf::Color(
	//					0, 
	//					0, 
	//					255 * actVal / theBiggestAbsActVal
	//				)
	//			);
	//		}
	//		else
	//		{
	//			m_neurons[i + 1][j].setFillColor(
	//				sf::Color(
	//					-255 * actVal / theBiggestAbsActVal, 
	//					0, 
	//					0
	//				)
	//			);
	//		}
	//	}
	//}
}

void gui::NeuralNetPreview::setOutputNeuronsColors()
{
	//typedef Blueberry::Scalar Scalar;
	//
	//Scalar theBiggestActVal = getTheBiggestActivatedValue(
	//	m_brain->getHiddenLayers().m_size() + 1
	//);
	//Scalar theSmallestActVal = getTheSmallestActivatedValue(
	//	m_brain->getHiddenLayers().m_size() + 1
	//);
	//
	//Scalar theBiggestAbsActVal = std::max(
	//	abs(theBiggestActVal), 
	//	abs(theSmallestActVal)
	//);
	//
	//auto outputLayer = m_brain->getOutputLayer();
	//
	//for (int i = 0; i < m_neurons.back().m_size(); i++)
	//{
	//	Scalar actVal = outputLayer->getNeurons()[i]->getActivatedValue();
	//
	//	if (actVal > 0.0)
	//	{
	//		m_neurons.back()[i].setFillColor(
	//			sf::Color(
	//				0, 
	//				0, 
	//				255 * actVal / theBiggestAbsActVal
	//			)
	//		);
	//	}
	//	else
	//	{
	//		m_neurons.back()[i].setFillColor(
	//			sf::Color(
	//				-255 * actVal / theBiggestAbsActVal, 
	//				0, 
	//				0
	//			)
	//		);
	//	}
	//}
}

Blueberry::Scalar gui::NeuralNetPreview::getTheBiggestActivatedValue(
	unsigned layerIndex)
{
	//// TODO: after implementing a new version of Bluebrry
	//// TODO: uncomment and rewrite this!:
	//
	//Blueberry::Scalar theBiggestActVal = -INFINITY;
	//
	//// layerIndex == 0 <==> we're dealing with the input layer:
	//if (!layerIndex)
	//{
	//	for (const auto& it : m_brain->getInputLayer()->output())
	//	{
	//		theBiggestActVal = std::max(theBiggestActVal, it);
	//	}
	//
	//	return theBiggestActVal;
	//}
	//
	//// layerIndex == hidden layers count + 1 
	//// <==> we're dealing with the output layer:
	//if (layerIndex == m_brain->getHiddenLayers().m_size() + 1)
	//{
	//	for (const auto& neuron : m_brain->getOutputLayer()->getNeurons())
	//	{
	//		theBiggestActVal = std::max(
	//			theBiggestActVal, 
	//			neuron->getActivatedValue()
	//		);
	//	}
	//
	//	return theBiggestActVal;
	//}
	//
	//auto hiddenLayers = m_brain->getHiddenLayers();
	//
	//// otherwise we're dealing with a hidden layer:
	//for (const auto& neuron : hiddenLayers[layerIndex - 1]->getNeurons())
	//{
	//	theBiggestActVal = std::max(
	//		theBiggestActVal, 
	//		neuron->getActivatedValue()
	//	);
	//}
	//
	//return theBiggestActVal;
	
	return 0.0;
}

Blueberry::Scalar gui::NeuralNetPreview::getTheSmallestActivatedValue(
	unsigned layerIndex)
{
	//// TODO: after implementing a new version of Blueberry
	//// TODO: uncomment and rewrite this!:
	//
	//Blueberry::Scalar theSmallestActVal = INFINITY;
	//
	//// layerIndex == 0 <==> we're dealing with the input layer:
	//if (!layerIndex)
	//{
	//	for (const auto& it : m_brain->getInputLayer()->output())
	//	{
	//		theSmallestActVal = std::min(theSmallestActVal, it);
	//	}
	//
	//	return theSmallestActVal;
	//}
	//
	//// layerIndex == hidden layers count + 1 <==> we're dealing with the output layer:
	//if (layerIndex == m_brain->getHiddenLayers().m_size() + 1)
	//{
	//	for (const auto& neuron : m_brain->getOutputLayer()->getNeurons())
	//	{
	//		theSmallestActVal = std::min(
	//			theSmallestActVal, 
	//			neuron->getActivatedValue()
	//		);
	//	}
	//
	//	return theSmallestActVal;
	//}
	//
	//auto hiddenLayers = m_brain->getHiddenLayers();
	//
	//// otherwise we're dealing with a hidden layer:
	//for (const auto& neuron : hiddenLayers[layerIndex - 1]->getNeurons())
	//{
	//	theSmallestActVal = std::min(
	//		theSmallestActVal, 
	//		neuron->getActivatedValue()
	//	);
	//}
	//
	//return theSmallestActVal;
	
	return 0.0;
}

void gui::NeuralNetPreview::setSynapsesPos()
{
	//// calculate useful variables:
	//unsigned hiddenLayersCount = m_brain->getHiddenLayers().m_size();
	//unsigned biggestLayerSize = getTheBiggestLayerSize();
	//
	//float gapBetweenLayers = calcGapBetweenLayers();
	//float radius = calcNeuronsRadius();
	//float diameter = 2.f * radius;
	//
	//// set positions:
	//for (int i = 0; i < m_synapses.m_size(); i++)
	//{
	//	float topMargin2 = calcTopMargin(i);
	//
	//	for (int j = 0; j < m_synapses[i].m_size(); j++)
	//	{
	//		float topMargin1 = calcTopMargin(j);
	//
	//		for (int k = 0; k < m_synapses[i][j].m_size(); k++)
	//		{
	//			float y1 = m_neurons[i][k].getPosition().y + radius;
	//			float y2 = m_neurons[i + 1][j].getPosition().y + radius;
	//
	//			float x1 = m_background.getPosition().x + 2 * diameter
	//					   + i * (gapBetweenLayers + diameter);
	//			float x2 = x1 + gapBetweenLayers;
	//
	//			m_synapses[i][j][k][0].position = sf::Vector2f(x1, y1);
	//			m_synapses[i][j][k][1].position = sf::Vector2f(x2, y2);
	//		}
	//	}
	//}
}

void gui::NeuralNetPreview::setSynapsesColors()
{
	//Blueberry::Scalar theBiggestWeight = getTheBiggestWeight();
	//Blueberry::Scalar theSmallestWeight = getTheSmallestWeight();
	//
	//Blueberry::Scalar theBiggestAbsWeight = std::max(
	//	abs(theBiggestWeight), 
	//	abs(theSmallestWeight)
	//);
	//
	//auto weights = m_brain->getWeights();
	//
	//for (int i = 0; i < m_synapses.m_size(); i++)
	//{
	//	for (int j = 0; j < m_synapses[i].m_size(); j++)
	//	{
	//		for (int k = 0; k < m_synapses[i][j].m_size(); k++)
	//		{
	//			Blueberry::Scalar weight = weights[i]->getValues()[j][k];
	//
	//			if (weight > 0.0)
	//			{
	//				m_synapses[i][j][k][0].color = sf::Color(
	//					0, 
	//					0, 
	//					255, 
	//					255 * weight / theBiggestAbsWeight
	//				);
	//				m_synapses[i][j][k][1].color = sf::Color(
	//					0, 
	//					0, 
	//					255, 
	//					255 * weight / theBiggestAbsWeight
	//				);
	//			}
	//			else
	//			{
	//				m_synapses[i][j][k][0].color = sf::Color(
	//					255, 
	//					0, 
	//					0, 
	//					255 * weight / theBiggestAbsWeight
	//				);
	//				m_synapses[i][j][k][1].color = sf::Color(
	//					255, 
	//					0, 
	//					0, 
	//					255 * weight / theBiggestAbsWeight
	//				);
	//			}
	//		}
	//	}
	//}
}

Blueberry::Scalar gui::NeuralNetPreview::getTheBiggestWeight()
{
	//// TODO: after implementing a new version Blueberry
	//// TODO: uncomment and rewrite this!:
	//
	//Blueberry::Scalar theBiggestWeight = -INFINITY;
	//
	//for (int i = 0; i < m_synapses.m_size(); i++)
	//{
	//	for (int j = 0; j < m_synapses[i].m_size(); j++)
	//	{
	//		for (int k = 0; k < m_synapses[i][j].m_size(); k++)
	//		{
	//			theBiggestWeight = std::max(
	//				theBiggestWeight, 
	//				m_brain->getWeights()[i]->getValues()[j][k]
	//			);
	//		}
	//	}
	//}
	//
	//return theBiggestWeight;
	
	return 0.0;
}

Blueberry::Scalar gui::NeuralNetPreview::getTheSmallestWeight()
{
	//// TODO: after implementing a new version of Blueberry
	//// TODO: CrappyNeuralNets uncomment and rewrite this!:
	//
	//Blueberry::Scalar theSmallestWeight = INFINITY;
	//
	//for (int i = 0; i < m_synapses.m_size(); i++)
	//{
	//	for (int j = 0; j < m_synapses[i].m_size(); j++)
	//	{
	//		for (int k = 0; k < m_synapses[i][j].m_size(); k++)
	//		{
	//			theSmallestWeight = std::min(
	//				theSmallestWeight, 
	//				m_brain->getWeights()[i]->getValues()[j][k]
	//			);
	//		}
	//	}
	//}
	//
	//return theSmallestWeight;
	
	return 0.0;
}

void gui::NeuralNetPreview::updateNeurons()
{
	setNeuronsColors();
}

void gui::NeuralNetPreview::renderNeurons(sf::RenderTarget& target) const
{
	for (const auto& layer : m_neurons)
	{
		for (const auto& neuron : layer)
		{
			target.draw(neuron);
		}
	}
}

void gui::NeuralNetPreview::renderSynapses(sf::RenderTarget& target) const
{
	for (const auto& matrix : m_synapses)
	{
		for (const auto& vector : matrix)
		{
			for (const auto& synapse : vector)
			{
				target.draw(&synapse[0], synapse.size(), sf::Lines);
			}
		}
	}
}
