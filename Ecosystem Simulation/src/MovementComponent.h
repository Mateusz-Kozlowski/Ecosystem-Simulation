#pragma once

#include "Libraries.h"

class MovementComponent
{
public:
	MovementComponent();
	MovementComponent(
		const sf::Vector2i& defaultVelocity,
		const char* brainFilePath
	);
	MovementComponent(const MovementComponent& rhs);
	MovementComponent& operator=(const MovementComponent& rhs);

	void saveBrainToFile(const char* filePath) const;
	void loadBrainFromFile(const char* filePath);

	void update(
		float dt,
		unsigned availableEnergy,
		float speedFactor,
		const std::vector<Blueberry::Scalar>& brainInputs,
		bool allowUserInput,
		std::ofstream& debugFile
	);

	// accessors:

	const Blueberry::Brain& getBrain() const;

	unsigned getEnergyToExpel() const;
	int getKineticEnergyDelta() const;
	unsigned getPreviousKineticEnergy() const;
	unsigned getKineticEnergy() const;

	unsigned getPreviousVelocityVectorSquaredValue() const;
	unsigned getVelocityVectorSquaredValue() const;
	float getAccelerationVectorValue() const;

	const sf::Vector2i& getPreviousVelocityVector() const;
	const sf::Vector2i& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	// mutators:

	void mutateBrain(unsigned brainMutationsCount);
	void anabolicBrainMutation(unsigned brainMutationsCount);
	void nonCatabolicBrainMutation(unsigned brainMutationsCount);

	//void setVelocity(const sf::Vector2i& velocity);
	//void setVelocity(int x, int y);

	void elasticReboundInAxisX();
	void elasticReboundInAxisY();

	// temporary; MovementComponent can have it's own file so that it can be loaded directly from it
	// the function doesn't change prevVel
	// the function doesn't change amount of energy to expel (look: getEnergyToExpel() const)
	void setVelocitiesLoadedFromFile(
		const sf::Vector2i& velocity, 
		const sf::Vector2i& prevVel
	);

	// resets velocity to 0
	// doesn't change previousVelocity 
	// (so doesn't impact energy to expel)
	void resetVelocity();

private:
	void updateAcceleration(
		const std::vector<Blueberry::Scalar>& brainInputs,
		bool allowUserInput,
		std::ofstream& debugFile
	);
	void handleUserInput();

	bool accelerationIsImpossible(
		float dt, 
		unsigned availableEnergy
	);
	static float getVectorValue(const sf::Vector2f& vector);
	// simulation = true if it's only checking if acceleration is possible:
	static float getVectorSquaredValue(
		const sf::Vector2i& vector,
		bool simulation,
		std::string origin,
		const sf::Vector2f& acceleration
	);

	void velocityGuard() const;

	void randomNumbersGeneratingGuard() const;

private:
	std::unique_ptr<Blueberry::Brain> m_brain;

	// kinematics:

	// velocity after previous brain output;
	// doesn't take into account fuctions like resetVelocity or loadVelocitiesFromFile 
	// (they don't change this variable)
	sf::Vector2i m_prevVelocity;
	
	sf::Vector2i m_velocity;
	sf::Vector2f m_acceleration;
};
