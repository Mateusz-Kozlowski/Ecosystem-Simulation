#include "stdafx.h"
#include "Individual.h"

// public static methods:
void Individual::setUpIndividualFolder(const std::string& folder_path)
{
	std::filesystem::create_directories(folder_path);

	MovementComponent movementComponent;

	movementComponent.saveToFolder(folder_path);
}

// initialization:
void Individual::loadFromFolder(const std::string& folder_path)
{	
	this->movementComponent.loadFromFolder(folder_path);

	this->shape.setPosition(this->movementComponent.get_x(), this->movementComponent.get_y());
	this->shape.setFillColor(sf::Color::Red);
	this->shape.setRadius(8.f);
	this->shape.setPointCount(16.f);
}

// accessors:
MovementComponent& Individual::getMovementComponent()
{
	return this->movementComponent;
}

void Individual::update(float dt, const std::vector<double>& brain_inputs)
{
	this->movementComponent.update(dt, brain_inputs);

	this->shape.setPosition(this->movementComponent.get_x(), this->movementComponent.get_y());
}

void Individual::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}
