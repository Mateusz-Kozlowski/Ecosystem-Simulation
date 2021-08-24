#pragma once

#include "MovementComponent.h"
#include "ProgressBar.h"
#include "BrainPreview.h"

class Animal
{
public:
	Animal(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& bodyColor,
		const sf::Color& hpBarBackgroundColor,
		const sf::Color& hpBarProgressRectColor,
		double defaultHp,
		double maxHp
	);
	Animal(const char* folderPath);
	Animal(const Animal& rhs);
	Animal& operator=(const Animal& rhs);

	void saveToFolder(const char* folderPath) const;
	void loadFromFolder(const char* folderPath);

	void update(
		float dt,
		float simulationSpeedFactor,
		const std::vector<double>& brainInputs
	);

	void renderBody(sf::RenderTarget& target) const;
	void renderHpBar(sf::RenderTarget& target) const;
	void renderBrainPreview(sf::RenderTarget& target) const;

	// accessors:

	const sf::Vector2f& getPosition() const;

	float getRadius() const;

	const sf::Color& getColor() const;

	double getMaxHp() const;

	const MovementComponent& getMovementComponent() const;

	const Blueberry::Brain& getBrain() const;
	
	double getEnergyToExpel() const;
	double getKineticEnergyDelta() const;
	double getPreviousKineticEnergy() const;
	double getKineticEnergy() const;

	float getPreviousVelocityVectorValue() const;
	float getVelocityVectorValue() const;
	float getAccelerationVectorValue() const;

	const sf::Vector2f& getPreviousVelocityVector() const;
	const sf::Vector2f& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	bool isAlive() const;

	double getHp() const;
	double getTotalEnergy() const;

	const gui::BrainPreview& getBrainPreview() const;

	float getTimeElapsedSinceLastExternalHpChange() const;

	bool isCoveredByMouse(const sf::Vector2f& mousePosView) const;

	// mutators:

	void setPosition(const sf::Vector2f& position);

	// TODO: Animal class shouldn't know about sth like world, 
	// TODO: change arguments to range:
	void setRandomPosition(
		const sf::Vector2f& worldSize,
		float bordersThickness
	);

	void setRadius(float radius);

	void setColor(const sf::Color& color);

	void setMaxHp(double maxHp);

	void randomMutate(unsigned brainMutationsCount);

	void setVelocity(const sf::Vector2f& velocity);

	void setAlive(bool alive);

	void setHp(double hp);
	//void increaseHp(double hpIncrease);
	//void decreaseHp(double hpDecrease);

	void setBrainPreviewPosition(const sf::Vector2f& position);
	void setBrainPreviewPosition(float x, float y);

private:
	void initBody(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	void initHpBar(
		double defaultHp,
		const sf::Color& hpBarBackgroundColor,
		const sf::Color& hpBarProgressRectColor
	);
	void initBrainPreview();

	void updateBody(float dt);
	void updateHp(float dt);
	void updateHpBarPosition();
	void updateBrainPreview();

private:
	sf::CircleShape m_body;

	double m_maxHp;

	std::unique_ptr<MovementComponent> m_movementComponent;
	
	bool m_alive;

	std::unique_ptr<gui::ProgressBar> m_hpBar;
	std::unique_ptr<gui::BrainPreview> m_brainPreview;

	float m_timeElapsedSinceLastExternalHpChange;
};
