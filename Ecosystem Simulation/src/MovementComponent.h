#pragma once

#include "Libraries.h"

class MovementComponent
{
public:
	MovementComponent();
	MovementComponent(
		const sf::Vector2f& defaultVelocity,
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
		bool allowUserInput
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

	void setVelocity(const sf::Vector2i& velocity);
	void setVelocity(unsigned x, unsigned y);

	void setVelocityX(unsigned vx);
	void setVelocityY(unsigned vy);

private:
	void updateAcceleration(
		const std::vector<Blueberry::Scalar>& brainInputs,
		bool allowUserInput
	);
	void handleUserInput();

	bool accelerationIsImpossible(
		float dt, 
		unsigned availableEnergy
	);
	static float getVectorValue(const sf::Vector2f& vector);
	static float getVectorSquaredValue(const sf::Vector2i& vector);

private:
	std::unique_ptr<Blueberry::Brain> m_brain;

	// kinematics:

	sf::Vector2i m_prevVelocity;
	sf::Vector2i m_velocity;
	sf::Vector2f m_acceleration;
};
