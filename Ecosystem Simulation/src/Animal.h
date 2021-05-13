#pragma once

#include "RenderingComponent.h"
#include "ProgressBar.h"

class Animal
{
public:
	// public static methods:
	static void setUpAnimalFolder(const std::string& folder_path);

	// constructor/destructor:
	Animal();
	~Animal();

	// initialization:
	void loadFromFolder(const std::string& folder_path);

	// accessors:
	sf::Vector2f getPos() const;
	sf::Vector2f getVelocity() const;
	float getRadius() const;
	
	bool isBrainRendered() const;
	
	bool isAlive() const;

	float getHp() const;

	// mutators:
	void setVelocity(const sf::Vector2f& v);

	void setBrainIsRendered(bool brain_is_rendered);

	void setHp(float new_hp);

	// other public methods:
	void update(float dt, const std::vector<double>& brain_inputs);
	void renderBody(sf::RenderTarget& target);
	void renderBrain(sf::RenderTarget& target);

private:
	MovementComponent* movementComponent;
	
	RenderingComponent* renderingComponent;
	float radius; // TODO: "un-hard-code" radius
	
	bool brainIsRendered;

	ProgressBar* hpBar;

	bool alive;

	// private utilities:
	void updateHpBar(float dt);
};
