#pragma once

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
		float speedFactor,
		const std::vector<double>& brainInputs
	);

	// accessors:

	const Blueberry::Brain& getBrain() const;

	const sf::Vector2f& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	float getVelocityVectorValue() const;
	float getAccelerationVectorValue() const;

	// mutators:

	void mutateBrain(unsigned brainMutationsCount);

	void setVelocity(const sf::Vector2f& velocity);
	void setVelocity(float x, float y);

	void setVelocityX(float vx);
	void setVelocityY(float vy);

private:
	std::unique_ptr<Blueberry::Brain> m_brain;

	// kinematics:

	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
};
