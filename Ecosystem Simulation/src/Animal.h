#pragma once

#include "MovementComponent.h"
#include "ProgressBar.h"
#include "NeuralNetPreview.h"

class Animal
{
public:
	Animal(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& body_color,
		const sf::Color& hp_bar_background_color,
		const sf::Color& hp_bar_progress_rect_color,
		float default_hp,
		float max_hp
	);
	Animal(const std::string& folder_path);
	Animal(const Animal& rhs);

	Animal& operator=(const Animal& rhs);

	// public methods:
	void saveToFolder(const std::string& folder_path) const;
	void loadFromFolder(const std::string& folder_path);

	void update(
		float dt,
		float simulation_speed_factor,
		const std::vector<double>& brain_inputs
	);

	void renderBody(sf::RenderTarget& target) const;
	void renderHpBar(sf::RenderTarget& target) const;
	void renderBrainPreview(sf::RenderTarget& target) const;

	// accessors:
	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	const sf::Color& getColor() const;
	
	float getHp() const;
	float getMaxHp() const;

	const MovementComponent& getMovementComponent() const;

	const CrappyNeuralNets::TempNet& getBrain() const;

	const sf::Vector2f& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	bool isAlive() const;

	bool isCoveredByMouse(const sf::Vector2f& mouse_pos_view) const;

	float getValueOfVelocityVector() const;

	float getValueOfAccelerationVector() const;

	float getKineticEnergy() const;

	float getTotalEnergy() const;

	// mutators:
	void setPosition(const sf::Vector2f& position);

	void setRandomPosition(const sf::Vector2f& worldSize, float bordersThickness);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

	void setHp(float hp);

	void increaseHp(float hp_increase);

	void decreaseHp(float hp_decrease);

	void setMaxHp(float max_hp);

	void setVelocity(const sf::Vector2f& velocity);

	void randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage);

private:
	sf::CircleShape body;

	float maxHp;

	std::unique_ptr<MovementComponent> movementComponent;

	bool alive;

	std::unique_ptr<gui::ProgressBar> hpBar;
	std::unique_ptr<gui::NeuralNetPreview> brainPreview;

	// private methods:
	// initialization:
	void initBody(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	void initHpBar(
		float default_hp,
		float max_hp,
		const sf::Color& hp_bar_background_color,
		const sf::Color& hp_bar_progress_rect_color
	);
	void initBrainPreview();

	// private utilities:
	void updateBody(float dt);
	void updateHp(float dt);
	void updateHpBarPosition();
	void updateBrainPreview();
};
