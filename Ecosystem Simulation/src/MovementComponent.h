#pragma once

#include "Crappy Neural Nets/NeuralNet.h"

class MovementComponent
{
public:
	// constructor:
	MovementComponent();

	// mutators:
	void set_x(float x);
	void set_y(float y);
	
	void set_vx(float vx);
	void set_vy(float vy);
	
	void set_ax(float ax);
	void set_ay(float ay);

	// accessors:
	const CrappyNeuralNets::NeuralNet& getBrain() const;

	const sf::Vector2f& getPos() const;
	
	const sf::Vector2f& getVelocity() const;

	// TODO: replace all of this with sf::Vectors:
	float get_x() const;
	float get_y() const;

	float get_vx() const;
	float get_vy() const;
	
	float get_ax() const;
	float get_ay() const;

	// other public methods:
	void update(float dt, const std::vector<double>& brain_inputs);

	void saveToFolder(const std::string& folder_path) const;
	void loadFromFolder(const std::string& folder_path);

private:
	CrappyNeuralNets::NeuralNet brain;

	// kinematics stuff:
	sf::Vector2f pos;
	sf::Vector2f v;
	sf::Vector2f a;
};
