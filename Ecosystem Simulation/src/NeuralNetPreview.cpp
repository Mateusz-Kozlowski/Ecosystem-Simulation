#include "stdafx.h"
#include "NeuralNetPreview.h"

using namespace gui;

NeuralNetPreview::NeuralNetPreview(
	const CrappyNeuralNets::TempNet& brain,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& background_color)
	: brain(&brain)
{
	this->initBackground(position, size, background_color);
	this->initNeurons();
	this->initSynapses();
}

// public methods:
void NeuralNetPreview::update()
{
	this->updateNeurons();
}

void NeuralNetPreview::render(sf::RenderTarget& target) const
{
	target.draw(this->background);

	this->renderNeurons(target);
	this->renderSynapses(target);
}

// accessors:
const CrappyNeuralNets::TempNet& NeuralNetPreview::getBrain() const
{
	return *this->brain;
}

const sf::Vector2f& NeuralNetPreview::getPosition() const
{
	return this->background.getPosition();
}

const sf::Vector2f& NeuralNetPreview::getSize() const
{
	return this->background.getSize();
}

const sf::Color& NeuralNetPreview::getBackgroundColor() const
{
	return this->background.getFillColor();
}

// mutators:
void NeuralNetPreview::setPosition(const sf::Vector2f& position)
{
	this->background.setPosition(position);

	this->setNeuronsPositions();
	this->setSynapsesPositions();
}

void NeuralNetPreview::setSize(const sf::Vector2f& size)
{
	this->background.setSize(size);

	this->setNeuronsPositions();
	this->setNeuronsSizes();
	this->setSynapsesPositions();
}

void NeuralNetPreview::setBackgroundColor(const sf::Color& color)
{
	this->background.setFillColor(color);
}

// private methods:
// initialization:
void NeuralNetPreview::initBackground(
	const sf::Vector2f& preview_position, 
	const sf::Vector2f& size, 
	const sf::Color& background_color)
{
	this->background.setPosition(preview_position);
	this->background.setSize(size);
	this->background.setFillColor(background_color);
}

void NeuralNetPreview::initNeurons()
{
	this->initNeuronsVector();
	this->setNeuronsPositions();
	this->setNeuronsSizes();
	this->setNeuronsColors();
}

void NeuralNetPreview::initNeuronsVector()
{
	/*
	unsigned hiddenLayersCount = this->brain->getHiddenLayers().size();

	this->neurons.resize(hiddenLayersCount + 2U);

	// resize input layer:
	this->neurons[0].resize(
		this->brain->getInputLayer()->getSize(),
		sf::CircleShape(0.0f, neurons_shapes_point_count)
	);

	// resize hidden layers:
	for (int i = 0; i < hiddenLayersCount; i++)
		this->neurons[i + 1].resize(
			this->brain->getHiddenLayers()[i]->getNeuronsCount(),
			sf::CircleShape(0.0f, neurons_shapes_point_count)
		);

	// resize output layer:
	this->neurons.back().resize(
		this->brain->getOutputLayer()->getNeuronsCount(),
		sf::CircleShape(0.0f, neurons_shapes_point_count)
	);
	*/
}

void NeuralNetPreview::initSynapses()
{
	this->initSynapsesVector();
	this->setSynapsesPositions();
	this->setSynapsesColors();
}

void NeuralNetPreview::initSynapsesVector()
{
	/*
	unsigned hiddenLayersCount = this->brain->getHiddenLayers().size();

	this->synapses.resize(hiddenLayersCount + 1U);

	// resize each matrix in synapses vector (make synapses a 2D structure):
	for (int i = 0; i < hiddenLayersCount; i++)
		this->synapses[i].resize(this->brain->getHiddenLayers()[i]->getNeuronsCount());

	this->synapses.back().resize(this->brain->getOutputLayer()->getNeuronsCount());

	// make synapses a 3D structure:
	if (hiddenLayersCount > 0)
	{
		for (int i = 0; i < this->brain->getHiddenLayers()[0]->getNeuronsCount(); i++)
			this->synapses[0][i].resize(this->brain->getInputLayer()->getSize());

		for (int i = 0; i < hiddenLayersCount - 1; i++)
			for (int j = 0; j < this->brain->getHiddenLayers()[i + 1]->getNeuronsCount(); j++)
				this->synapses[i + 1][j].resize(this->brain->getHiddenLayers()[i]->getNeuronsCount());

		for (int i = 0; i < this->brain->getOutputLayer()->getNeuronsCount(); i++)
			this->synapses.back()[i].resize(this->brain->getHiddenLayers().back()->getNeuronsCount());
	}
	else
	{
		for (int i = 0; i < this->brain->getOutputLayer()->getNeuronsCount(); i++)
			this->synapses.back()[i].resize(this->brain->getInputLayer()->output().size());
	}

	// each synapse consists of 2 vertices:
	for (auto& matrix : this->synapses)
		for (auto& vector : matrix)
			for (auto& synapse : vector)
				synapse.resize(2);
	*/
}

// private utilities:
void NeuralNetPreview::setNeuronsPositions()
{
	for (int i = 0; i < this->neurons.size(); i++)
	{
		for (int j = 0; j < this->neurons[i].size(); j++)
		{
			this->neurons[i][j].setPosition(this->calcNeuronPosition(i, j));
		}
	}

}

sf::Vector2f NeuralNetPreview::calcNeuronPosition(unsigned index1, unsigned index2) const
{
	sf::Vector2f result;

	float gapBetweenLayers = this->calcGapBetweenLayers();
	float topMargin = this->calcTopMargin(index1);
	float diameter = 2.f * this->calcNeuronsRadius();

	result.x += this->background.getPosition().x + diameter + index1 * (diameter + gapBetweenLayers);
	result.y += this->background.getPosition().y + topMargin + 2U * index2 * diameter;

	return result;
}

float NeuralNetPreview::calcGapBetweenLayers() const
{
	// TODO: after implementing new CrappyNeuralNets uncomment and rewrite this!:
	/*
	unsigned hiddenLayersCount = this->brain->getHiddenLayers().size();
	float diameter = 2.f * this->calcNeuronsRadius();

	return (this->background.getSize().x - (4U + hiddenLayersCount) * diameter) / (hiddenLayersCount + 1U);
	*/
	return 0.0f;
}

float NeuralNetPreview::calcNeuronsRadius() const
{
	float diameter = this->background.getSize().y / (2U * this->getTheBiggestLayerSize() + 1U);

	return diameter / 2.f;
}

float NeuralNetPreview::calcTopMargin(unsigned index1) const
{
	float diameter = 2.f * this->calcNeuronsRadius();

	return (this->background.getSize().y - (2 * this->neurons[index1].size() - 1) * diameter) / 2.f;
}

unsigned NeuralNetPreview::getTheBiggestLayerSize() const
{
	// TODO: after implementing new CrappyNeuralNets uncomment and rewrite this!:
	/*
	// input layer:
	unsigned getTheBiggestLayerSize = this->brain->getInputLayer()->getSize();

	// hidden layers:
	for (const auto& hiddenLayer : this->brain->getHiddenLayers())
		getTheBiggestLayerSize = std::max(getTheBiggestLayerSize, hiddenLayer->getNeuronsCount());

	// output layer:
	getTheBiggestLayerSize = std::max(getTheBiggestLayerSize, this->brain->getOutputLayer()->getNeuronsCount());

	return getTheBiggestLayerSize;
	*/
	return 0U;
}

void NeuralNetPreview::setNeuronsSizes()
{
	unsigned getTheBiggestLayerSize = this->getTheBiggestLayerSize();
	float radius = this->calcNeuronsRadius();

	for (auto& it1 : this->neurons)
		for (auto& it2 : it1)
			it2.setRadius(radius);
}

void NeuralNetPreview::setNeuronsColors()
{
	this->setInputNeuronsColors();
	this->setHiddenNeuronsColors();
	this->setOutputNeuronsColors();
}

void NeuralNetPreview::setInputNeuronsColors()
{
	/*
	CrappyNeuralNets::Scalar theBiggestActVal = this->getTheBiggestActivatedValue(0U);
	CrappyNeuralNets::Scalar theSmallestActVal = this->getTheSmallestActivatedValue(0U);

	CrappyNeuralNets::Scalar theBiggestAbsActVal = std::max(abs(theBiggestActVal), abs(theSmallestActVal));

	for (int i = 0; i < this->neurons[0].size(); i++)
	{
		CrappyNeuralNets::Scalar neuronActVal = this->brain->getInputLayer()->output()[i];

		if (neuronActVal > 0.0)
			this->neurons[0][i].setFillColor(sf::Color(0, 0, 255 * neuronActVal / theBiggestAbsActVal, 255));
		else
			this->neurons[0][i].setFillColor(sf::Color(-255 * neuronActVal / theBiggestAbsActVal, 0, 0, 255));
	}
	*/
}

void NeuralNetPreview::setHiddenNeuronsColors()
{
	/*
	for (int i = 0; i < this->brain->getHiddenLayers().size(); i++)
	{
		CrappyNeuralNets::Scalar theBiggestActVal = this->getTheBiggestActivatedValue(i + 1);
		CrappyNeuralNets::Scalar theSmallestActVal = this->getTheSmallestActivatedValue(i + 1);

		CrappyNeuralNets::Scalar theBiggestAbsActVal = std::max(abs(theBiggestActVal), abs(theSmallestActVal));

		for (int j = 0; j < this->neurons[i + 1].size(); j++)
		{
			CrappyNeuralNets::Scalar actVal = this->brain->getHiddenLayers()[i]->getNeurons()[j]->getActivatedValue();

			if (actVal > 0.0)
				this->neurons[i + 1][j].setFillColor(sf::Color(0, 0, 255 * actVal / theBiggestAbsActVal));
			else
				this->neurons[i + 1][j].setFillColor(sf::Color(-255 * actVal / theBiggestAbsActVal, 0, 0));
		}
	}
	*/
}

void NeuralNetPreview::setOutputNeuronsColors()
{
	/*
	CrappyNeuralNets::Scalar theBiggestActVal = this->getTheBiggestActivatedValue(this->brain->getHiddenLayers().size() + 1);
	CrappyNeuralNets::Scalar theSmallestActVal = this->getTheSmallestActivatedValue(this->brain->getHiddenLayers().size() + 1);

	CrappyNeuralNets::Scalar theBiggestAbsActVal = std::max(abs(theBiggestActVal), abs(theSmallestActVal));

	for (int i = 0; i < this->neurons.back().size(); i++)
	{
		CrappyNeuralNets::Scalar actVal = this->brain->getOutputLayer()->getNeurons()[i]->getActivatedValue();

		if (actVal > 0.0)
			this->neurons.back()[i].setFillColor(sf::Color(0, 0, 255 * actVal / theBiggestAbsActVal));
		else
			this->neurons.back()[i].setFillColor(sf::Color(-255 * actVal / theBiggestAbsActVal, 0, 0));
	}
	*/
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheBiggestActivatedValue(unsigned layer_index)
{
	// TODO: after implementing new CrappyNeuralNets uncomment and rewrite this!:
	/*
	CrappyNeuralNets::Scalar theBiggestActVal = -INFINITY;

	// layer_index == 0 <==> we're dealing with the input layer:
	if (!layer_index)
	{
		for (const auto& it : this->brain->getInputLayer()->output())
			theBiggestActVal = std::max(theBiggestActVal, it);

		return theBiggestActVal;
	}

	// layer_index == hidden layers count + 1 <==> we're dealing with the output layer:
	if (layer_index == this->brain->getHiddenLayers().size() + 1)
	{
		for (const auto& neuron : this->brain->getOutputLayer()->getNeurons())
			theBiggestActVal = std::max(theBiggestActVal, neuron->getActivatedValue());

		return theBiggestActVal;
	}

	// otherwise we're dealing with a hidden layer:
	for (const auto& neuron : this->brain->getHiddenLayers()[layer_index - 1]->getNeurons())
		theBiggestActVal = std::max(theBiggestActVal, neuron->getActivatedValue());

	return theBiggestActVal;
	*/
	return 0.0;
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheSmallestActivatedValue(unsigned layer_index)
{
	// TODO: after implementing new CrappyNeuralNets uncomment and rewrite this!:
	/*
	CrappyNeuralNets::Scalar theSmallestActVal = INFINITY;

	// layer_index == 0 <==> we're dealing with the input layer:
	if (!layer_index)
	{
		for (const auto& it : this->brain->getInputLayer()->output())
			theSmallestActVal = std::min(theSmallestActVal, it);

		return theSmallestActVal;
	}

	// layer_index == hidden layers count + 1 <==> we're dealing with the output layer:
	if (layer_index == this->brain->getHiddenLayers().size() + 1)
	{
		for (const auto& neuron : this->brain->getOutputLayer()->getNeurons())
			theSmallestActVal = std::min(theSmallestActVal, neuron->getActivatedValue());

		return theSmallestActVal;
	}

	// otherwise we're dealing with a hidden layer:
	for (const auto& neuron : this->brain->getHiddenLayers()[layer_index - 1]->getNeurons())
		theSmallestActVal = std::min(theSmallestActVal, neuron->getActivatedValue());

	return theSmallestActVal;
	*/
	return 0.0;
}

void NeuralNetPreview::setSynapsesPositions()
{
	/*
	// calculate useful variables:
	unsigned hiddenLayersCount = this->brain->getHiddenLayers().size();
	unsigned biggestLayerSize = this->getTheBiggestLayerSize();

	float gapBetweenLayers = this->calcGapBetweenLayers();
	float radius = this->calcNeuronsRadius();
	float diameter = 2.f * radius;

	// set positions:
	for (int i = 0; i < this->synapses.size(); i++)
	{
		float topMargin2 = this->calcTopMargin(i);

		for (int j = 0; j < this->synapses[i].size(); j++)
		{
			float topMargin1 = this->calcTopMargin(j);

			for (int k = 0; k < this->synapses[i][j].size(); k++)
			{
				float y1 = this->neurons[i][k].getPosition().y + radius;
				float y2 = this->neurons[i + 1][j].getPosition().y + radius;

				float x1 = this->background.getPosition().x + 2 * diameter + i * (gapBetweenLayers + diameter);
				float x2 = x1 + gapBetweenLayers;

				this->synapses[i][j][k][0].position = sf::Vector2f(x1, y1);
				this->synapses[i][j][k][1].position = sf::Vector2f(x2, y2);
			}
		}
	}
	*/
}

void NeuralNetPreview::setSynapsesColors()
{
	/*
	CrappyNeuralNets::Scalar theBiggestWeight = this->getTheBiggestWeight();
	CrappyNeuralNets::Scalar theSmallestWeight = this->getTheSmallestWeight();

	CrappyNeuralNets::Scalar theBiggestAbsWeight = std::max(abs(theBiggestWeight), abs(theSmallestWeight));

	for (int i = 0; i < this->synapses.size(); i++)
		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
			{
				CrappyNeuralNets::Scalar weight = this->brain->getWeights()[i]->getValues()[j][k];

				if (weight > 0.0)
				{
					this->synapses[i][j][k][0].color = sf::Color(0, 0, 255, 255 * weight / theBiggestAbsWeight);
					this->synapses[i][j][k][1].color = sf::Color(0, 0, 255, 255 * weight / theBiggestAbsWeight);
				}
				else
				{
					this->synapses[i][j][k][0].color = sf::Color(255, 0, 0, 255 * weight / theBiggestAbsWeight);
					this->synapses[i][j][k][1].color = sf::Color(255, 0, 0, 255 * weight / theBiggestAbsWeight);
				}
			}
	*/
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheBiggestWeight()
{
	// TODO: after implementing new CrappyNeuralNets uncomment and rewrite this!:
	/*
	CrappyNeuralNets::Scalar theBiggestWeight = -INFINITY;

	for (int i = 0; i < this->synapses.size(); i++)
		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
				theBiggestWeight = std::max(theBiggestWeight, this->brain->getWeights()[i]->getValues()[j][k]);

	return theBiggestWeight;
	*/
	return 0.0;
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheSmallestWeight()
{
	// TODO: after implementing new CrappyNeuralNets uncomment and rewrite this!:
	/*
	CrappyNeuralNets::Scalar theSmallestWeight = INFINITY;

	for (int i = 0; i < this->synapses.size(); i++)
		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
				theSmallestWeight = std::min(theSmallestWeight, this->brain->getWeights()[i]->getValues()[j][k]);

	return theSmallestWeight;
	*/
	return 0.0;
}

void NeuralNetPreview::updateNeurons()
{
	this->setNeuronsColors();
}

void NeuralNetPreview::renderNeurons(sf::RenderTarget& target) const
{
	for (const auto& layer : this->neurons)
		for (const auto& neuron : layer)
			target.draw(neuron);
}

void NeuralNetPreview::renderSynapses(sf::RenderTarget& target) const
{
	for (const auto& matrix : synapses)
		for (const auto& vector : matrix)
			for (const auto& synapse : vector)
				target.draw(&synapse[0], synapse.size(), sf::Lines);
}
