#pragma once

#include "MovementComponent.h"

class Individual
{
public:
	// public static methods:
	static void setUpIndividualFolder(const std::string& folder_path);

	// initialization:
	void loadFromFolder(const std::string& folder_path);
	
	// accessors:
	MovementComponent& getMovementComponent();

	// other public methods:
	void update(float dt, const std::vector<double>& brain_inputs);
	void render(sf::RenderTarget& target);

private:
	sf::CircleShape shape;

	MovementComponent movementComponent;
};

