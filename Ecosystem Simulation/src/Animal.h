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
		int defaultHp,
		unsigned basalMetabolicRatePerFrame
	);
	Animal(const char* folderPath);
	Animal(const Animal& rhs);
	Animal& operator=(const Animal& rhs);

	void saveToFolder(const char* folderPath) const;
	void loadFromFolder(const char* folderPath);

	void update(
		float dt,
		float simulationSpeedFactor,
		const std::vector<Blueberry::Scalar>& externalInputsForBrain,
		bool isTracked,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events,
		std::ofstream& debugFile,
		const std::unordered_map<std::string, int>& keybinds
	);
	void updateOnlyImgBtnOfBrainPreview(
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);

	void renderBody(sf::RenderTarget& target) const;
	void renderHpBar(sf::RenderTarget& target) const;
	void renderBrainPreview(sf::RenderTarget& target) const;

	std::string toStr() const;

	// accessors:

	const sf::Vector2f& getPos() const;

	float getRadius() const;

	const sf::Color& getColor() const;
	
	MovementComponent& getMovementComponent() const;

	const Blueberry::Brain& getBrain() const;
	
	unsigned getEnergyToExpel() const; // cannot be negative (fruit with negative energy?)
	int getKineticEnergyDelta() const; // can be negative
	unsigned getPreviousKineticEnergy() const; // cannot be negative because it's a square
	unsigned getKineticEnergy() const; // cannot be negative because it's a square

	unsigned getPreviousVelocityVectorSquaredValue() const;
	unsigned getVelocityVectorSquaredValue() const;
	float getAccelerationVectorValue() const;

	const sf::Vector2i& getPreviousVelocityVector() const;
	const sf::Vector2i& getVelocityVector() const;
	const sf::Vector2f& getAccelerationVector() const;

	bool isAlive() const;
	bool isClone() const;
	float getParentAgeWhenItWasBorn() const;

	int getHp() const;
	// an animal cannot have total energy < 0, 
	// because what would we do after its death? create a fruit with negative energy?
	unsigned getTotalEnergy() const;
	
	const std::unique_ptr<gui::IntProgressBar>& getHpBar() const;

	gui::BrainPreview& getBrainPreview() const;

	float getAge() const;

	float getTimeElapsedSinceLastExternalHpChange() const;
	float getTimeElapsedSinceLastCloning() const;

	bool isCoveredByMouse(const sf::Vector2f& mousePos) const;

	unsigned getBasalMetabolicRatePerFrame() const;

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

	void randomMutate(
		unsigned brainMutationsCount,
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);

	void setAlive(bool alive);

	void setHp(int hp);
	//void increaseHp(const Blueberry::Scalar& hpIncrease);
	//void decreaseHp(const Blueberry::Scalar& hpDecrease);

	void setHpBarRange(const sf::Vector2i& range);

	void setBrainPreviewPosition(const sf::Vector2f& position);
	void setBrainPreviewPosition(float x, float y);

	void resetTimeElapsedSinceLastCloning();

	void setBasalMetabolicRatePerFrame(unsigned basalMetabolicRatePerFrame);

private:
	void initBody(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color
	);
	void initHpBar(
		int defaultHp,
		const sf::Color& hpBarBackgroundColor,
		const sf::Color& hpBarProgressRectColor
	);
	void initBrainPreview();
	
	void doBMRrelatedThings();

	std::vector<Blueberry::Scalar> getEnhancedBrainInputs(
		const std::vector<Blueberry::Scalar>& externalBrainInputs
	) const;
	
	void updateBody(float dt);
	void updateHp(float dt);
	void updateHpBarPosition();
	void updateBrainPreview(
		const sf::Vector2f& mousePos,
		const std::vector<sf::Event>& events
	);

private:
	sf::CircleShape m_body;

	std::unique_ptr<MovementComponent> m_movementComponent;
	
	bool m_alive;
	bool m_isClone;
	float m_parentAgeWhenItWasBorn;

	std::unique_ptr<gui::IntProgressBar> m_hpBar;
	std::unique_ptr<gui::BrainPreview> m_brainPreview;

	float m_age;
	float m_timeElapsedSinceLastExternalHpChange;
	float m_timeElapsedSinceLastCloning;

	unsigned m_basalMetabolicRatePerFrame;
	unsigned m_energyToExpelFromBMR;
};
