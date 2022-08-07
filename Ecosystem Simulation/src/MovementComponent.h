#pragma once

#include "Brain.h"

class MovementComponent
{
public:
	MovementComponent();
	MovementComponent(
		const sf::Vector2i& defaultVelocity,
		unsigned brainInputsCount,
		unsigned brainOutputsCount,
		const std::vector<std::string>& additionalInfo,
		float velocityCoeffcient = 10.0f,
		bool randomMutateOutputBiases = true
	);
	MovementComponent(
		const char* brainFilePath,
		const sf::Vector2i& defaultVelocity = sf::Vector2i(0, 0),
		float velocityCoeffcient = 10.0f
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
		std::ofstream& debugFile,
		const std::unordered_map<std::string, int>& keybinds
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

	float getVelocityCoefficient() const;

	// mutators:

	void mutateBrain(unsigned brainMutationsCount);

	//void setVelocity(const sf::Vector2i& velocity);
	//void setVelocity(int x, int y);

	void elasticReboundInAxisX();
	void elasticReboundInAxisY();

	// temporary; MovementComponent can have it's own file so that it can be loaded directly from it
	// the function doesn't change amount of energy to expel (look: getEnergyToExpel() const)
	void setVelocitiesVectorsLoadedFromFile(
		const sf::Vector2i& velocity, 
		const sf::Vector2i& prevVel
	);

	// temporary; MovementComponent can have it's own file so that it can be loaded directly from it
	void setAccelerationVectorLoadedFromFile(const sf::Vector2f& acceleration);

	// resets velocity to 0
	// doesn't change previousVelocity 
	// (so doesn't impact energy to expel)
	void resetVelocity();

	void setVelocityCoefficient(float velocityCoeffcient);

private:
	void updateAcceleration(
		const std::vector<Blueberry::Scalar>& brainInputs,
		bool allowUserInput,
		std::ofstream& debugFile,
		const std::unordered_map<std::string, int>& keybinds
	);
	void handleUserInput(const std::unordered_map<std::string, int>& keybinds);

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

	void anabolicBrainMutation(unsigned brainMutationsCount);

private:
	std::unique_ptr<Blueberry::Brain> m_brain;

	// kinematics:

	// velocity after previous brain output;
	// doesn't take into account fuctions like resetVelocity or loadVelocitiesFromFile 
	// (they don't change this variable)
	sf::Vector2i m_prevVelocity;
	
	sf::Vector2i m_velocity;
	sf::Vector2f m_acceleration;

	float m_velocityCoefficient;
};
