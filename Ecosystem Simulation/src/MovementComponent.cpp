#include "stdafx.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent()
{	
	this->brain = std::make_unique<CrappyNeuralNets::TempNet>(CrappyNeuralNets::TempNet(5U, 2U));
}

MovementComponent::MovementComponent(const sf::Vector2f& default_velocity, const std::string& brain_file_path)
	: velocity(default_velocity)
{
	this->loadBrainFromFile(brain_file_path);
}

MovementComponent::MovementComponent(const MovementComponent& rhs)
{
	*this->brain = *rhs.brain;
	this->velocity = rhs.velocity;
	this->acceleration = rhs.acceleration;
}

MovementComponent& MovementComponent::operator=(const MovementComponent& rhs)
{
	if (this != &rhs)
	{
		*this->brain = *rhs.brain;
		this->velocity = rhs.velocity;
		this->acceleration = rhs.acceleration;
	}

	return *this;
}

// public methods:
void MovementComponent::saveBrainToFile(const std::string& file_path) const
{
	//this->BRAIN->saveToFile(file_path);
}

void MovementComponent::loadBrainFromFile(const std::string& file_path)
{
	//this->BRAIN->loadFromFile(file_path);
}

void MovementComponent::update(float dt, float speed_factor, const std::vector<double>& brain_inputs)
{
	// update acceleration:	
	this->brain->input(brain_inputs);
	const std::vector<double>& brainOutput = this->brain->output();

	// TODO: implement slowing down using speed_factor:

	// update acceleration:
	this->acceleration.x = brainOutput[0];
	this->acceleration.y = brainOutput[1];

	// update velocity:
	this->velocity.x += this->acceleration.x * dt;
	this->velocity.y += this->acceleration.y * dt;
}

// accessors:
const CrappyNeuralNets::TempNet& MovementComponent::getBrain() const
{
	return *this->brain;
}

const sf::Vector2f& MovementComponent::getVelocityVector() const
{
	return this->velocity;
}

const sf::Vector2f& MovementComponent::getAccelerationVector() const
{
	return this->acceleration;
}

float MovementComponent::get_vx() const
{
	return this->velocity.x;
}

float MovementComponent::get_vy() const
{
	return this->velocity.y;
}

float MovementComponent::get_ax() const
{
	return this->acceleration.x;
}

float MovementComponent::get_ay() const
{
	return this->acceleration.y;
}

float MovementComponent::getVelocityVectorValue() const
{
	return sqrt(pow(this->velocity.x, 2) + pow(this->velocity.y, 2));
}

float MovementComponent::getAccelerationVectorValue() const
{
	return sqrt(pow(this->acceleration.x, 2) + pow(this->acceleration.y, 2));
}

// mutators:
void MovementComponent::randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage)
{
	//this->BRAIN->randomMutate(mutation_percentage);
}

void MovementComponent::setVelocity(const sf::Vector2f& velocity)
{
	this->velocity = velocity;
}

void MovementComponent::setVelocity(float x, float y)
{
	this->velocity.x = x;
	this->velocity.y = y;
}

void MovementComponent::set_vx(float vx)
{
	this->velocity.x = vx;
}

void MovementComponent::set_vy(float vy)
{
	this->velocity.y = vy;
}
