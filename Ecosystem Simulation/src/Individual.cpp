#include "stdafx.h"
#include "Individual.h"

// public static methods:
void Individual::setUpIndividualFolder(const std::string& folder_path)
{
	std::filesystem::create_directories(folder_path);

	MovementComponent movementComponent;

	movementComponent.set_x(1920.f);
	movementComponent.set_y(1080.f);

	movementComponent.saveToFolder(folder_path);
}

// constructor:
Individual::Individual()
	: brainIsRendered(false)
{
	this->movementComponent = new MovementComponent();

	this->renderingComponent = new RenderingComponent(*this->movementComponent);

	this->radius = 8.f;
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
sf::Vector2f Individual::getPos() const
{
	return { this->movementComponent->get_x(), this->movementComponent->get_y() };
}

sf::Vector2f Individual::getVelocity() const
{
	return { this->movementComponent->get_vx(), this->movementComponent->get_vy() };
}

float Individual::getRadius() const
{
	return this->radius;
}

bool Individual::isBrainRendered() const
{
	return this->brainIsRendered;
}

// mutators:
void Individual::setVelocity(const sf::Vector2f& v)
{
	this->movementComponent->set_vx(v.x);
	this->movementComponent->set_vy(v.y);
}

void Individual::setBrainIsRendered(bool brain_is_rendered)
{
	this->brainIsRendered = brain_is_rendered;
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
