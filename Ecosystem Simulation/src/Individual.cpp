#include "stdafx.h"
#include "Individual.h"

// public static methods:
void Individual::setUpIndividualFolder(const std::string& folder_path)
{
	std::filesystem::create_directories(folder_path);

	MovementComponent movementComponent;

	movementComponent.saveToFolder(folder_path);
}

// constructor:
Individual::Individual()
	: brainIsRendered(true)
{
	std::cout << "NEW INDIVIDUAL <==> NEW MOVEMENT COMPONENT\n";

	this->movementComponent = new MovementComponent();

	std::cout << "v/v\n";

	this->renderingComponent = new RenderingComponent(*this->movementComponent);
}

Individual::~Individual()
{
	delete this->movementComponent;
	delete this->renderingComponent;
}

// initialization:
void Individual::loadFromFolder(const std::string& folder_path)
{	
	this->movementComponent->loadFromFolder(folder_path);
}

// accessors:
MovementComponent& Individual::getMovementComponent()
{
	return *this->movementComponent;
}

// other public methods:
void Individual::update(float dt, const std::vector<double>& brain_inputs)
{
	this->movementComponent->update(dt, brain_inputs);

	this->renderingComponent->updateBodyRedering(*this->movementComponent);
	
	if (this->brainIsRendered) this->renderingComponent->updateBrainRendering(*this->movementComponent);
}

void Individual::renderBody(sf::RenderTarget& target)
{
	this->renderingComponent->renderBody(target);
}

void Individual::renderBrain(sf::RenderTarget& target)
{
	if (this->brainIsRendered) this->renderingComponent->renderBrain(target);
}
