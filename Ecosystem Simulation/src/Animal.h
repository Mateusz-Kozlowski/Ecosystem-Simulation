#pragma once

#include "ProgressBar.h"
#include "NeuralNetPreview.h"
#include "MovementComponent.h"

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
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getVelocity() const;

	float getRadius() const;
	
	bool isBrainRendered() const;
	
	bool isAlive() const;

	float getHp() const;

	// mutators:
	void setPosition(const sf::Vector2f& new_pos);

	void setVelocity(const sf::Vector2f& v);

	void setBrainIsRendered(bool brain_is_rendered);

	void setHp(float hp);

	void increaseHp(float hp_increase);

	void setColor(const sf::Color& new_color);

	// other public methods:
	void updateBodyAndHp(float dt, float speed_factor, const std::vector<double>& brain_inputs);
	void updateBrainPreview();

	void renderBody(sf::RenderTarget& target) const ;
	void renderHpBar(sf::RenderTarget& target) const;
	void renderBrain(sf::RenderTarget& target) const;

	bool isCovered(const sf::Vector2f& mouse_pos_view) const;

private:
	bool alive;
	
	sf::CircleShape body;

	MovementComponent* movementComponent;

	float defaultHpValue;

	float hp;

	ProgressBar* hpBar;

	NeuralNetPreview* brainPreview;

	bool brainIsRendered;

	// private utilities:
	void updateHp(float dt, float speed_factor = 1.f);
};
