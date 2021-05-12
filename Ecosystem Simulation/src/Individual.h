#pragma once

#include "RenderingComponent.h"

class Individual
{
public:
	// public static methods:
	static void setUpIndividualFolder(const std::string& folder_path);

	// constructor/destructor:
	Individual();
	~Individual();

	// initialization:
	void loadFromFolder(const std::string& folder_path);

	// accessors:
	sf::Vector2f getPos() const;
	sf::Vector2f getVelocity() const;

	// mutators:
	void setVelocity(const sf::Vector2f& v);

	// other public methods:
	void update(float dt, const std::vector<double>& brain_inputs);
	void renderBody(sf::RenderTarget& target);
	void renderBrain(sf::RenderTarget& target);

private:
	MovementComponent* movementComponent;
	
	RenderingComponent* renderingComponent;
	
	bool brainIsRendered;
};
