#include "stdafx.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent()
{
	this->brain.initInputLayer(new CrappyNeuralNets::InputLayer(5U));
			   
	//this->brain.addHiddenLayer(new CrappyNeuralNets::HiddenLayer(4U, "fast sigmoid"));
	//this->brain.addHiddenLayer(new CrappyNeuralNets::HiddenLayer(3U, "relu"));
			   
	this->brain.initOutputLayer(new CrappyNeuralNets::OutputLayer(2U));
			   
	this->brain.compile();
}

void MovementComponent::copyConstructor(const MovementComponent& movement_component)
{
	this->brain.copyConstructor(movement_component.getBrain());
	this->position = movement_component.position;
	this->velocity = movement_component.velocity;
	this->acceleration = movement_component.acceleration;
}

// mutators:
void MovementComponent::set_x(float x)
{
	this->position.x = x;
}

void MovementComponent::set_y(float y)
{
	this->position.y = y;
}

void MovementComponent::set_vx(float vx)
{
	this->velocity.x = vx;
}

void MovementComponent::set_vy(float vy)
{
	this->velocity.y = vy;
}

void MovementComponent::set_ax(float ax)
{
	this->acceleration.x = ax;
}

void MovementComponent::set_ay(float ay)
{
	this->acceleration.y = ay;
}

void MovementComponent::randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage)
{
	this->brain.randomMutate(mutation_percentage);
}

// accessors:
const CrappyNeuralNets::NeuralNet& MovementComponent::getBrain() const
{
	return this->brain;
}

const sf::Vector2f& MovementComponent::getPosition() const
{
	return this->position;
}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return this->velocity;
}

float MovementComponent::get_x() const
{
	return this->position.x;
}

float MovementComponent::get_y() const
{
	return this->position.y;
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

void MovementComponent::update(float dt, float speed_factor, const std::vector<double>& brain_inputs)
{
	// update acceleration:
	const std::vector<double>& brainOutput = this->brain.predict(brain_inputs);

	this->acceleration.x = brainOutput[0];
	this->acceleration.y = brainOutput[1];

	// update velocity:
	this->velocity.x += this->acceleration.x * dt;
	this->velocity.y += this->acceleration.y * dt;

	// update positions:
	this->position.x += this->velocity.x * dt * speed_factor;
	this->position.y += this->velocity.y * dt * speed_factor;
}

void MovementComponent::saveToFolder(const std::string& folder_path) const
{
	std::filesystem::create_directories(folder_path);

	this->brain.saveToFile(folder_path + "/brain.ini");

	// save kinematics stuff:
	std::ofstream kinematicsFile(folder_path + "/kinematics stuff.ini");

	if (!kinematicsFile.is_open())
		throw("ERROR::MOVEMENTCOMPONENT::CANNOT OPEN A FILE: " + folder_path + "/kinematics stuff.ini");

	std::stringstream ss;
	
	ss << this->position.x << ' ' << this->position.y << '\n';
	ss << this->velocity.x << ' ' << this->velocity.y << '\n';
	ss << this->acceleration.x << ' ' << this->acceleration.y << '\n';

	kinematicsFile << ss.str();

	kinematicsFile.close();
}

void MovementComponent::loadFromFolder(const std::string& folder_path)
{
	this->brain.loadFromFile(folder_path + "/brain.ini");

	// load kinematics stuff:
	std::ifstream kinematicsFile(folder_path + "/kinematics stuff.ini");

	if (!kinematicsFile.is_open())
		throw("ERROR::MOVEMENTCOMPONENT::CANNOT OPEN A FILE: " + folder_path + "/kinematics stuff.ini");

	kinematicsFile >> this->position.x >> this->position.y;
	kinematicsFile >> this->velocity.x >> this->velocity.y;
	kinematicsFile >> this->acceleration.x >> this->acceleration.y;

	kinematicsFile.close();
}
