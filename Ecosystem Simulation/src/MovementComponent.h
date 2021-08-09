#pragma once

class MovementComponent
{
public:
	MovementComponent();
	MovementComponent(
		const sf::Vector2f& default_velocity, 
		const char* brain_file_path
	);
	MovementComponent(const MovementComponent& rhs);
	MovementComponent& operator=(const MovementComponent& rhs);
	
	void saveBrainToFile(const char* file_path) const;
	void loadBrainFromFile(const char* file_path);

	void update(
		float dt, 
		float speed_factor, 
		const std::vector<double>& brain_inputs
	);

	// accessors:

	const Blueberry::Brain& getBrain() const;

	const sf::Vector2f& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	float get_vx() const;
	float get_vy() const;

	float get_ax() const;
	float get_ay() const;

	float getVelocityVectorValue() const;
	float getAccelerationVectorValue() const;

	// mutators:

	void mutateBrain(unsigned brain_mutations_count);

	void setVelocity(const sf::Vector2f& velocity);
	void setVelocity(float x, float y);

	void set_vx(float vx);
	void set_vy(float vy);

private:
	std::unique_ptr<Blueberry::Brain> brain;

	// kinematics:

	sf::Vector2f velocity;
	sf::Vector2f acceleration;
};
