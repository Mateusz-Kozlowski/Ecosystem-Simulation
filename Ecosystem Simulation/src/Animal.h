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
	sf::Vector2f getPos() const;
	sf::Vector2f getVelocity() const;
	float getRadius() const;
	
	bool isBrainRendered() const;
	
	bool isAlive() const;

	float getMaxHp() const;

	float getHp() const;

	// mutators:
	void setPos(const sf::Vector2f& new_pos);

	void setVelocity(const sf::Vector2f& v);

	void setBrainIsRendered(bool brain_is_rendered);

	void setHp(float new_hp);

	// other public methods:
	void updateBodyAndHp(float dt, const std::vector<double>& brain_inputs);
	void updateBrainPreview();

	void renderBody(sf::RenderTarget& target) const ;
	void renderHpBar(sf::RenderTarget& target) const;
	void renderBrain(sf::RenderTarget& target) const;

	bool isMouseClickedOnIt(const sf::Vector2f& mouse_pos_view) const;

private:
	bool alive;
	
	sf::CircleShape body;

	MovementComponent* movementComponent;
	
	float maxHp;

	ProgressBar* hpBar;

	bool hpIsRendered;

	NeuralNetPreview* brainPreview;

	bool brainIsRendered;

	// private utilities:
	void updateHp(float dt);
};
