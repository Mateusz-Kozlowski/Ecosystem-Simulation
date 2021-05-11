#pragma once

#include "MovementComponent.h"

class RenderingComponent
{
public:
	// constructor:
	RenderingComponent(const MovementComponent& movementComponent);

	// other public methods:
	void updateBodyRedering(const MovementComponent& movementComponent);
	void updateBrainRendering(const MovementComponent& movementComponent);
	
	void renderBody(sf::RenderTarget& target);
	void renderBrain(sf::RenderTarget& target);

private:
	sf::CircleShape bodyShape;

	sf::RectangleShape brainRenderingBackground;

	std::vector<std::vector<sf::CircleShape>> neuronsShapes;
	
	// initialization:
	void initBodyRendering(const MovementComponent& movementComponent);
	void initBrainRendering(const MovementComponent& movementComponent);

	void initBrainBackground(const MovementComponent& movementComponent);
	void initNeuronsShapes(const MovementComponent& movementComponent);

	// utilities:
	CrappyNeuralNets::Scalar findMaxActivatedValue(const CrappyNeuralNets::NeuralNet& brain);
	CrappyNeuralNets::Scalar findMinActivatedValue(const CrappyNeuralNets::NeuralNet& brain);

	void updateNeuronsShapesPositions(const MovementComponent& movementComponent);
	void updateNeuronsShapesColors(const const CrappyNeuralNets::NeuralNet& brain);
};
