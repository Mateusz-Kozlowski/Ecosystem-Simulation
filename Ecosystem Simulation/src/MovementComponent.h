#pragma once

#include "Crappy Neural Nets/TempNet.h"

class MovementComponent
{
public:
	MovementComponent(const sf::Vector2f& default_velocity);
	MovementComponent(const sf::Vector2f& default_velocity, const std::string& brain_file_path);
	MovementComponent(const MovementComponent& rhs);

	MovementComponent& operator=(const MovementComponent& rhs);

	// public methods:
	void saveBrainToFile(const std::string& file_path) const;
	void loadBrainFromFile(const std::string& file_path);

	void update(float dt, float speed_factor, const std::vector<double>& brain_inputs);

	// accessors:
	const CrappyNeuralNets::TempNet& getBrain() const;

	const sf::Vector2f& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	float get_vx() const;
	float get_vy() const;

	float get_ax() const;
	float get_ay() const;

	float getValueOfVelocityVector() const;
	float getValueOfAccelerationVector() const;

	// mutators:
	void randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage);

	void setVelocity(const sf::Vector2f& velocity);
	void setVelocity(float x, float y);

	void set_vx(float vx);
	void set_vy(float vy);

private:
	std::unique_ptr<CrappyNeuralNets::TempNet> brain;

	// kinematics things:
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
};
