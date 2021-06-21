#pragma once

#include "MovementComponent.h"
#include "ProgressBar.h"
#include "NeuralNetPreview.h"

class Animal
{
public:
	// public static methods:
	static void setUpAnimalFolder(const std::string& folder_path);

	// constructor/destructor:
	Animal(float max_hp, float default_hp, bool hp_bar_is_rendered = true, bool brain_is_rendered = true);
	~Animal();

	void copyConstructor(const Animal& animal);

	// initialization:
	void loadFromFolder(const std::string& folder_path);

	// accessors:
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getVelocity() const;

	float getRadius() const;
	
	bool isHpBarRendered() const;

	bool isBrainRendered() const;
	
	bool isAlive() const;

	const MovementComponent& getMovementComponent() const;

	float getMaxHp() const;

	float getHp() const;

	const sf::Color& getColor() const;

	const CrappyNeuralNets::NeuralNet& getBrain() const;

	bool isCovered(const sf::Vector2f& mouse_pos_view) const;

	// mutators:
	void setPosition(const sf::Vector2f& new_position);
	void setVelocity(const sf::Vector2f& new_velocity);

	void setHpBarIsRendered(bool hp_bar_is_rendered);

	void setBrainIsRendered(bool brain_is_rendered);

	void setHp(float hp);

	void increaseHp(float hp_increase);

	void setColor(const sf::Color& new_color);

	void randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage);

	// other public methods:
	void update(float dt, float speed_factor, const std::vector<double>& brain_inputs);

	void renderBody(sf::RenderTarget& target) const;
	void renderHpBar(sf::RenderTarget& target) const;
	void renderBrainPreview(sf::RenderTarget& target) const;

	// TODO: rmv later!:
	std::string copied;

private:
	bool alive;
	
	sf::CircleShape body;

	MovementComponent* movementComponent;

	float maxHp;
	float hp;

	bool hpBarIsRendered;
	bool brainIsRendered;

	ProgressBar* hpBar;
	NeuralNetPreview* brainPreview;

	// private initialization:
	void initBody();
	void initMovementComponent();
	void initHpBar();
	void initBrainPreview();

	// private utilities:
	void updateHp(float dt, float speed_factor = 1.f);
	void updateHpBar();
};
