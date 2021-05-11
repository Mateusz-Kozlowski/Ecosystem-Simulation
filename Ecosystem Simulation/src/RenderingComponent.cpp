#include "stdafx.h"
#include "RenderingComponent.h"

// constructor:
RenderingComponent::RenderingComponent(const MovementComponent& movementComponent)
{
	this->initBodyRendering(movementComponent);
	this->initBrainRendering(movementComponent);
}

// other public methods:
void RenderingComponent::updateBodyRedering(const MovementComponent& movementComponent)
{
	this->bodyShape.setPosition(movementComponent.get_x(), movementComponent.get_y());
}

void RenderingComponent::updateBrainRendering(const MovementComponent& movementComponent)
{
	this->brainRenderingBackground.setPosition(movementComponent.get_x(), movementComponent.get_y());

	this->updateNeuronsShapesPositions(movementComponent);
	this->updateNeuronsShapesColors(movementComponent.getBrain());
}

void RenderingComponent::renderBody(sf::RenderTarget& target)
{
	target.draw(this->bodyShape);
}

void RenderingComponent::renderBrain(sf::RenderTarget& target)
{
	target.draw(this->brainRenderingBackground);

	for (const auto& it1 : this->neuronsShapes) for (const auto& it2 : it1) target.draw(it2);
}

// private methods:
// initialization:
void RenderingComponent::initBodyRendering(const MovementComponent& movementComponent)
{
	this->bodyShape.setPointCount(16);
	this->bodyShape.setRadius(8.f);
}

void RenderingComponent::initBrainRendering(const MovementComponent& movementComponent)
{
	this->initBrainBackground(movementComponent);
	this->initNeuronsShapes(movementComponent);
}

void RenderingComponent::initBrainBackground(const MovementComponent& movementComponent)
{
	this->brainRenderingBackground.setFillColor(sf::Color(255, 255, 0, 24));
	this->brainRenderingBackground.setSize(sf::Vector2f(120.f, 120.f));
}

void RenderingComponent::initNeuronsShapes(const MovementComponent& movementComponent)
{
	unsigned layersCount = movementComponent.getBrain().getHiddenLayers().size() + 2;

	this->neuronsShapes.resize(layersCount);

	for (int i = 0; i < this->neuronsShapes.size(); i++)
	{
		unsigned size = 0U;
		
		// check the size of a layer:
		if (!i) size = movementComponent.getBrain().getInputLayer()->getSize();

		else if (i == movementComponent.getBrain().getHiddenLayers().size() + 1)
			size = movementComponent.getBrain().getOutputLayer()->getNeuronsCount();

		else size = movementComponent.getBrain().getHiddenLayers()[i - 1]->getNeuronsCount();

		this->neuronsShapes[i].resize(size);
		
		for (int j = 0; j < this->neuronsShapes[i].size(); j++)
		{
			this->neuronsShapes[i][j].setPointCount(16.f);
			this->neuronsShapes[i][j].setRadius(8.f);
		}
	}
}

// utilities:
CrappyNeuralNets::Scalar RenderingComponent::findMaxActivatedValue(const CrappyNeuralNets::NeuralNet& brain)
{
	CrappyNeuralNets::Scalar maxValue = brain.getInputLayer()->output()[0];

	// input layer:
	for (int i = 1; i < brain.getInputLayer()->getSize(); i++)
		maxValue = std::max(maxValue, brain.getInputLayer()->output()[i]);

	// hidden layers:
	for (int i = 0; i < brain.getHiddenLayers().size(); i++)
		for (int j = 0; j < brain.getHiddenLayers()[i]->getNeuronsCount(); j++)
			maxValue = std::max(maxValue, brain.getHiddenLayers()[i]->getNeurons()[j]->getActivatedValue());

	// output layer:
	for (int i = 1; i < brain.getOutputLayer()->getNeuronsCount(); i++)
		maxValue = std::max(maxValue, brain.getOutputLayer()->getNeurons()[i]->getActivatedValue());

	return maxValue;
}

CrappyNeuralNets::Scalar RenderingComponent::findMinActivatedValue(const CrappyNeuralNets::NeuralNet& brain)
{
	CrappyNeuralNets::Scalar minValue = brain.getInputLayer()->output()[0];

	// input layer:
	for (int i = 1; i < brain.getInputLayer()->getSize(); i++)
		minValue = std::min(minValue, brain.getInputLayer()->output()[i]);

	// hidden layers:
	for (int i = 0; i < brain.getHiddenLayers().size(); i++)
		for (int j = 0; j < brain.getHiddenLayers()[i]->getNeuronsCount(); j++)
			minValue = std::min(minValue, brain.getHiddenLayers()[i]->getNeurons()[j]->getActivatedValue());

	// output layer:
	for (int i = 1; i < brain.getOutputLayer()->getNeuronsCount(); i++)
		minValue = std::min(minValue, brain.getOutputLayer()->getNeurons()[i]->getActivatedValue());

	return minValue;
}

void RenderingComponent::updateNeuronsShapesPositions(const MovementComponent& movementComponent)
{
	for (int i = 0; i < this->neuronsShapes.size(); i++)
	{
		for (int j = 0; j < this->neuronsShapes[i].size(); j++)
		{
			this->neuronsShapes[i][j].setPosition(
				movementComponent.get_x() + 2.f * i * 16.f,
				movementComponent.get_y() + 1.5f * j * 16.f
			);
		}
	}
}

void RenderingComponent::updateNeuronsShapesColors(const const CrappyNeuralNets::NeuralNet& brain)
{
	// find max and min value:
	CrappyNeuralNets::Scalar maxValue = this->findMaxActivatedValue(brain);
	CrappyNeuralNets::Scalar minValue = this->findMinActivatedValue(brain);

	CrappyNeuralNets::Scalar absMax = std::max(abs(maxValue), abs(maxValue));

	// input layer:
	for (int i = 0; i < brain.getInputLayer()->getSize(); i++)
	{
		CrappyNeuralNets::Scalar value = brain.getInputLayer()->output()[i];

		if (value > 0.0) this->neuronsShapes[0][i].setFillColor(sf::Color(0, 0, 255 * value / absMax));

		else this->neuronsShapes[0][i].setFillColor(sf::Color(255 * value / absMax, 0, 0));
	}

	// hidden layers:
	for (int i = 0; i < brain.getHiddenLayers().size(); i++)
	{
		for (int j = 0; j < brain.getHiddenLayers()[i]->getNeuronsCount(); j++)
		{
			CrappyNeuralNets::Scalar value = brain.getHiddenLayers()[i]->getNeurons()[j]->getActivatedValue();

			if (value > 0.0) this->neuronsShapes[i + 1][j].setFillColor(sf::Color(0, 0, 255 * value / absMax));

			else this->neuronsShapes[i + 1][j].setFillColor(sf::Color(255 * value / absMax, 0, 0));
		}
	}

	// output layers:
	for (int i = 0; i < brain.getOutputLayer()->getNeuronsCount(); i++)
	{
		CrappyNeuralNets::Scalar value = brain.getOutputLayer()->getNeurons()[i]->getActivatedValue();

		if (value > 0.0) this->neuronsShapes.back()[i].setFillColor(sf::Color(0, 0, 255 * value / absMax));

		else this->neuronsShapes.back()[i].setFillColor(sf::Color(255 * value / absMax, 0, 0));
	}
}
