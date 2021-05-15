#include "stdafx.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent()
{
	this->brain.initInputLayer(new CrappyNeuralNets::InputLayer(5U));
			   
	this->brain.addHiddenLayer(new CrappyNeuralNets::HiddenLayer(4U, "fast sigmoid"));
	this->brain.addHiddenLayer(new CrappyNeuralNets::HiddenLayer(3U, "tanh"));
			   
	this->brain.initOutputLayer(new CrappyNeuralNets::OutputLayer(2U));
			   
	this->brain.compile();
}

// mutators:
void MovementComponent::set_x(float x)
{
	this->pos.x = x;
}

void MovementComponent::set_y(float y)
{
	this->pos.y = y;
}

void MovementComponent::set_vx(float vx)
{
	this->v.x = vx;
}

void MovementComponent::set_vy(float vy)
{
	this->v.y = vy;
}

void MovementComponent::set_ax(float ax)
{
	this->a.x = ax;
}

void MovementComponent::set_ay(float ay)
{
	this->a.y = ay;
}

// accessors:
const CrappyNeuralNets::NeuralNet& MovementComponent::getBrain() const
{
	return this->brain;
}

float MovementComponent::get_x() const
{
	return this->pos.x;
}

float MovementComponent::get_y() const
{
	return this->pos.y;
}

float MovementComponent::get_vx() const
{
	return this->v.x;
}

float MovementComponent::get_vy() const
{
	return this->v.y;
}

float MovementComponent::get_ax() const
{
	return this->a.x;
}

float MovementComponent::get_ay() const
{
	return this->a.y;
}

void MovementComponent::update(float dt, const std::vector<double>& brain_inputs)
{
	// update acceleration:
	const std::vector<double>& brainOutput = this->brain.predict(brain_inputs);

	this->a.x = brainOutput[0];
	this->a.y = brainOutput[1];

	//std::cout << this->a.x << ' ' << this->a.y << '\n';

	// update velocity:
	this->v.x += this->a.x * dt;
	this->v.y += this->a.y * dt;
	
	// update positions:
	this->pos.x += this->v.x * dt;
	this->pos.y += this->v.y * dt;
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
	
	ss << this->pos.x << ' ' << this->pos.y << '\n';
	ss << this->v.x << ' ' << this->v.y << '\n';
	ss << this->a.x << ' ' << this->a.y << '\n';

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

	kinematicsFile >> this->pos.x >> this->pos.y;
	kinematicsFile >> this->v.x >> this->v.y;
	kinematicsFile >> this->a.x >> this->a.y;

	kinematicsFile.close();
}
