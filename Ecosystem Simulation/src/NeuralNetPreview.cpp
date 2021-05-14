#include "stdafx.h"
#include "NeuralNetPreview.h"

// constructor:
NeuralNetPreview::NeuralNetPreview(
	const CrappyNeuralNets::NeuralNet& brain, 
	const sf::Vector2f& pos, 
	const sf::Vector2f& size, 
	const sf::Color& background_color)
	: brain(brain)
{
	this->initBackground(pos, size, background_color);
	this->initNeurons(pos, size);
	this->initSynapses(pos, size);
}

// other public methods:
void NeuralNetPreview::update(const sf::Vector2f& new_pos)
{
	this->updateNeurons(new_pos);
	this->updateSynapses(new_pos);
}

void NeuralNetPreview::render(sf::RenderTarget& target) const
{
	target.draw(this->background);
	
	this->renderNeurons(target);
	this->renderSynapses(target);
}

// private methods:
// initialization:
void NeuralNetPreview::initBackground(const sf::Vector2f& preview_pos, const sf::Vector2f& size, const sf::Color& background_color)
{
	this->background.setPosition(preview_pos);
	this->background.setSize(size);
	this->background.setFillColor(background_color);
}

void NeuralNetPreview::initNeurons(const sf::Vector2f& preview_pos, const sf::Vector2f& size)
{
	// calculate useful variables:
	unsigned hiddenLayersCount = this->brain.getHiddenLayers().size();

	unsigned biggestLayerSize = this->biggestLayerSize();

	float diameter = size.y / (2U * biggestLayerSize + 1U);

	// resize:
	this->neurons.resize(hiddenLayersCount + 2U);

	// resize input layer:
	this->neurons[0].resize(this->brain.getInputLayer()->getSize());

	// resize hidden layer:
	for (int i = 0; i < hiddenLayersCount; i++)
		this->neurons[i + 1].resize(this->brain.getHiddenLayers()[i]->getNeuronsCount());

	// resize output layer:
	this->neurons.back().resize(this->brain.getOutputLayer()->getNeuronsCount());

	// set point count:
	for (int i = 0; i < this->neurons.size(); i++)
		for (int j = 0; j < this->neurons[i].size(); j++)
		{
			this->neurons[i][j].setPointCount(16);
			this->neurons[i][j].setRadius(diameter / 2);
		}
}

void NeuralNetPreview::initSynapses(const sf::Vector2f& preview_pos, const sf::Vector2f& size)
{
	// calculate useful variables:
	unsigned hiddenLayersCount = this->brain.getHiddenLayers().size();

	unsigned biggestLayerSize = this->biggestLayerSize();

	float diameter = size.y / (2U * biggestLayerSize + 1U);

	float gapBetweenLayers = (size.x - (4 + hiddenLayersCount) * diameter) / (hiddenLayersCount + 1);

	// resize:
	this->synapses.resize(this->brain.getHiddenLayers().size() + 1U);

	// resize each matrix in synapses vector:
	for (int i = 0; i < hiddenLayersCount; i++)
		this->synapses[i].resize(this->brain.getHiddenLayers()[i]->getNeuronsCount());

	this->synapses.back().resize(this->brain.getOutputLayer()->getNeuronsCount());

	for (int i = 0; i < this->brain.getHiddenLayers()[0]->getNeuronsCount(); i++)
		this->synapses[0][i].resize(this->brain.getInputLayer()->getSize());

	for (int i = 0; i < hiddenLayersCount - 1; i++)
		for (int j = 0; j < this->brain.getHiddenLayers()[i + 1]->getNeuronsCount(); j++)
			this->synapses[i + 1][j].resize(this->brain.getHiddenLayers()[i]->getNeuronsCount());

	for (int i = 0; i < this->brain.getOutputLayer()->getNeuronsCount(); i++)
		this->synapses.back()[i].resize(this->brain.getHiddenLayers().back()->getNeuronsCount());

	// now set up each synapse rectangle size and rotation:
	for (int i = 0; i < this->synapses.size(); i++)
	{
		float topMargin2 = size.y - (2 * this->neurons[i].size() - 1) * diameter;

		for (int j = 0; j < this->synapses[i].size(); j++)
		{
			float topMargin1 = size.y - (2 * this->neurons[j].size() - 1) * diameter;

			for (int k = 0; k < this->synapses[i][j].size(); k++)
			{
				float y1 = topMargin1 + diameter + 2 * k * diameter + diameter / 2;
				float y2 = topMargin2 + diameter + 2 * j * diameter + diameter / 2;

				float a = gapBetweenLayers;
				float b = y1 - y2; // it should be abs(y1 - y2), but it will be squared in a moment, so finally it does not matter

				// Pythagorean theorem:
				float c = sqrt(pow(a, 2) + pow(b, 2));

				this->synapses[i][j][k].setSize(sf::Vector2f(c, 1));
				this->synapses[i][j][k].setRotation(atan(b / a));
			}
		}
	}

	// their colors won't ever change as long as animals cannot mutate during its lives:
	this->setSynapsesColors();
}

// private utilities:
unsigned NeuralNetPreview::biggestLayerSize() const
{
	// input layer:
	unsigned biggestLayerSize = this->brain.getInputLayer()->getSize();

	// hidden layers:
	for (const auto& hiddenLayer : this->brain.getHiddenLayers())
		biggestLayerSize = std::max(biggestLayerSize, hiddenLayer->getNeuronsCount());

	// output layer:
	biggestLayerSize = std::max(biggestLayerSize, this->brain.getOutputLayer()->getNeuronsCount());

	return biggestLayerSize;
}

void NeuralNetPreview::updateNeurons(const sf::Vector2f& new_preview_pos)
{
	this->setNeuronsPositions(new_preview_pos);
	this->setNeuronsColors();
}

void NeuralNetPreview::updateSynapses(const sf::Vector2f& new_preview_pos)
{
	this->setSynapsesPositions(new_preview_pos);
}

void NeuralNetPreview::setNeuronsPositions(const sf::Vector2f& preview_pos)
{
	// calculate useful variables:
	unsigned hiddenLayersCount = this->brain.getHiddenLayers().size();

	unsigned biggestLayerSize = this->biggestLayerSize();

	float diameter = this->background.getSize().y / (2U * biggestLayerSize + 1U);

	float gapBetweenLayers = (this->background.getSize().x - (4 + hiddenLayersCount) * diameter) / (hiddenLayersCount + 1);

	// set up positions:
	for (int i = 0; i < this->neurons.size(); i++)
	{
		float topMargin = this->background.getSize().y - (2 * this->neurons[i].size() - 1) * diameter;

		for (int j = 0; j < this->neurons[i].size(); j++)
		{
			float x = preview_pos.x + diameter + i * (diameter + gapBetweenLayers);
			float y = preview_pos.y + topMargin + 2 * j * diameter;

			this->neurons[i][j].setPosition(x, y);
		}
	}
}

void NeuralNetPreview::setSynapsesPositions(const sf::Vector2f& preview_pos)
{
	// calculate useful variables:
	unsigned hiddenLayersCount = this->brain.getHiddenLayers().size();

	unsigned biggestLayerSize = this->biggestLayerSize();

	float diameter = this->background.getSize().y / (2U * biggestLayerSize + 1U);

	float gapBetweenLayers = (this->background.getSize().x - (4 + hiddenLayersCount) * diameter) / (hiddenLayersCount + 1);

	// set positions:
	for (int i = 0; i < this->synapses.size(); i++)
	{
		float topMargin = this->background.getSize().y - (2 * this->neurons[i].size() - 1) * diameter;

		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
			{
				float x = preview_pos.x + diameter + i * (diameter + gapBetweenLayers);
				float y = preview_pos.y + topMargin + diameter + 2 * k * diameter + diameter / 2;

				this->synapses[i][j][k].setPosition(x, y);
			}
	}
}

void NeuralNetPreview::setNeuronsColors()
{
	// input layer:
	CrappyNeuralNets::Scalar theBiggestActVal = this->getTheBiggestActivatedValue(0);
	CrappyNeuralNets::Scalar theSmallestActVal = this->getTheSmallestActivatedValue(0);

	CrappyNeuralNets::Scalar theBiggestAbsVal = std::max(abs(theBiggestActVal), abs(theSmallestActVal));

	for (int i = 0; i < this->neurons[0].size(); i++)
	{
		CrappyNeuralNets::Scalar actVal = this->brain.getInputLayer()->output()[i];

		if (actVal > 0.0) this->neurons[0][i].setFillColor(sf::Color(0, 0, 255 * actVal / theBiggestAbsVal));

		else this->neurons[0][i].setFillColor(sf::Color(-255 * actVal / theBiggestAbsVal, 0, 0));
	}

	// hidden layers:
	for (int i = 0; i < this->brain.getHiddenLayers().size(); i++)
	{
		CrappyNeuralNets::Scalar theBiggestActVal = this->getTheBiggestActivatedValue(i + 1);
		CrappyNeuralNets::Scalar theSmallestActVal = this->getTheSmallestActivatedValue(i + 1);

		CrappyNeuralNets::Scalar theBiggestAbsVal = std::max(abs(theBiggestActVal), abs(theSmallestActVal));

		for (int j = 0; j < this->neurons[i + 1].size(); j++)
		{
			CrappyNeuralNets::Scalar actVal = this->brain.getHiddenLayers()[i]->getNeurons()[j]->getActivatedValue();

			if (actVal > 0.0) this->neurons[i][j].setFillColor(sf::Color(0, 0, 255 * actVal / theBiggestAbsVal));

			else this->neurons[i][j].setFillColor(sf::Color(-255 * actVal / theBiggestAbsVal, 0, 0));
		}
	}

	// output layer:
	CrappyNeuralNets::Scalar theBiggestActVal = this->getTheBiggestActivatedValue(this->brain.getHiddenLayers().size() + 1);
	CrappyNeuralNets::Scalar theSmallestActVal = this->getTheSmallestActivatedValue(this->brain.getHiddenLayers().size() + 1);

	CrappyNeuralNets::Scalar theBiggestAbsVal = std::max(abs(theBiggestActVal), abs(theSmallestActVal));

	for (int i = 0; i < this->neurons.back().size(); i++)
	{
		CrappyNeuralNets::Scalar actVal = this->brain.getOutputLayer()->getNeurons()[i]->getActivatedValue();

		if (actVal > 0.0) this->neurons.back()[i].setFillColor(sf::Color(0, 0, 255 * actVal / theBiggestAbsVal));

		else this->neurons.back()[i].setFillColor(sf::Color(-255 * actVal / theBiggestAbsVal, 0, 0));
	}
}

void NeuralNetPreview::setSynapsesColors()
{
	CrappyNeuralNets::Scalar theBiggestWeight = this->getTheBiggestWeight();
	CrappyNeuralNets::Scalar theSmallestWeight = this->getTheSmallestWeight();

	CrappyNeuralNets::Scalar theBiggestAbsVal = std::max(abs(theBiggestWeight), abs(theSmallestWeight));

	for (int i = 0; i < this->synapses.size(); i++)
		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
			{
				CrappyNeuralNets::Scalar weight = this->brain.getWeights()[i]->getValues()[j][k];

				if (weight > 0.0) this->synapses[i][j][k].setFillColor(sf::Color(0, 0, 255, 255 * weight / theBiggestAbsVal));

				else this->synapses[i][j][k].setFillColor(sf::Color(255, 0, 0, 255 * weight / theBiggestAbsVal));
			}
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheBiggestActivatedValue(unsigned layer_index)
{
	CrappyNeuralNets::Scalar theBiggestActVal = -INFINITY;

	if (!layer_index)
	{
		for (const auto& it : this->brain.getInputLayer()->output())
			theBiggestActVal = std::max(theBiggestActVal, it);

		return theBiggestActVal;
	}
	if (layer_index == this->brain.getHiddenLayers().size() + 1)
	{
		for (const auto& neuron : this->brain.getOutputLayer()->getNeurons())
			theBiggestActVal = std::max(theBiggestActVal, neuron->getActivatedValue());

		return theBiggestActVal;
	}

	for (const auto& neuron : this->brain.getHiddenLayers()[layer_index - 1]->getNeurons())
		theBiggestActVal = std::max(theBiggestActVal, neuron->getActivatedValue());

	return theBiggestActVal;
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheSmallestActivatedValue(unsigned layer_index)
{
	CrappyNeuralNets::Scalar theSmallestActVal = INFINITY;

	if (!layer_index)
	{
		for (const auto& it : this->brain.getInputLayer()->output())
			theSmallestActVal = std::min(theSmallestActVal, it);

		return theSmallestActVal;
	}
	if (layer_index == this->brain.getHiddenLayers().size() + 1)
	{
		for (const auto& neuron : this->brain.getOutputLayer()->getNeurons())
			theSmallestActVal = std::min(theSmallestActVal, neuron->getActivatedValue());

		return theSmallestActVal;
	}

	for (const auto& neuron : this->brain.getHiddenLayers()[layer_index - 1]->getNeurons())
		theSmallestActVal = std::min(theSmallestActVal, neuron->getActivatedValue());

	return theSmallestActVal;
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheBiggestWeight()
{
	CrappyNeuralNets::Scalar theBiggestWeight = -INFINITY;

	for (int i = 0; i < this->synapses.size(); i++)
		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
				theBiggestWeight = std::max(theBiggestWeight, this->brain.getWeights()[i]->getValues()[j][k]);
	
	return theBiggestWeight;
}

CrappyNeuralNets::Scalar NeuralNetPreview::getTheSmallestWeight()
{
	CrappyNeuralNets::Scalar theSmallestWeight = INFINITY;

	for (int i = 0; i < this->synapses.size(); i++)
		for (int j = 0; j < this->synapses[i].size(); j++)
			for (int k = 0; k < this->synapses[i][j].size(); k++)
				theSmallestWeight = std::min(theSmallestWeight, this->brain.getWeights()[i]->getValues()[j][k]);

	return theSmallestWeight;
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
				target.draw(synapse);
}
