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

	for (int i=0; i<this->neuronsShapes.size(); i++)
	{
		for (int j = 0; j < this->neuronsShapes[i].size(); j++)
		{
			this->neuronsShapes[i][j].setPosition(
				movementComponent.get_x() + 2.f * i * 16.f,
				movementComponent.get_y() + 1.5f * j * 16.f
			);

			//CrappyNeuralNets::Scalar value = 0.0;

			/*
			if (!i) this->neuronsShapes[i][j].setFillColor(movementComponent.getBrain().getInputLayer().get);

			else if(i == this->neuronsShapes.size() - 1)
				this->neuronsShapes[i][j].setFillColor(movementComponent.getBrain().getInputLayer().get);

			else
			{
				value = movementComponent.getBrain().getHiddenLayers()[i - 1]->getNeurons()[j]->getActivatedValue();
			}
			*/

			this->neuronsShapes[i][j].setFillColor(sf::Color::Red);
		}
	}
}

void RenderingComponent::renderBody(sf::RenderTarget& target)
{
	target.draw(this->bodyShape);
}

void RenderingComponent::renderBrain(sf::RenderTarget& target)
{
	target.draw(this->brainRenderingBackground);

	for (const auto& it1 : this->neuronsShapes) 
		for (const auto& it2 : it1)
			target.draw(it2);			
}

// initialization:
void RenderingComponent::initBodyRendering(const MovementComponent& movementComponent)
{
	this->bodyShape.setPointCount(16);
	this->bodyShape.setRadius(8.f);
}

void RenderingComponent::initBrainRendering(const MovementComponent& movementComponent)
{
	this->brainRenderingBackground.setFillColor(sf::Color(255, 255, 0, 24));
	this->brainRenderingBackground.setSize(sf::Vector2f(120.f, 120.f));

	for (int i = 0; i < movementComponent.getBrain().getHiddenLayers().size() + 2; i++)
	{
		this->neuronsShapes.push_back(std::vector<sf::CircleShape>());

		unsigned size = 0U;

		if (!i)
			size = movementComponent.getBrain().getInputLayer()->getSize();

		else if (i == movementComponent.getBrain().getHiddenLayers().size() + 1)
			size = movementComponent.getBrain().getOutputLayer()->getNeuronsCount();

		else
			size = movementComponent.getBrain().getHiddenLayers()[i - 1]->getNeuronsCount();

		for (int j = 0; j < size; j++)
		{
			this->neuronsShapes[i].push_back(sf::CircleShape());
			this->neuronsShapes[i][j].setPointCount(16.f);
			this->neuronsShapes[i][j].setRadius(8.f);
		}
	}
}
