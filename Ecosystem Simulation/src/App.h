#pragma once

#include "MainMenuState.h"

class App
{
public:
	App();
	~App();
	
	void run();

private:
	GraphicsSettings gfxSettings;
	sf::RenderWindow* window;
	std::unordered_map<std::string, int> supportedKeys;
	std::stack<State*> states;
	Ecosystem* ecosystem;
	std::vector<sf::Event> events;

	StateData stateData;

	sf::Clock clock;
	float dt;

	sf::Event event;

	// private methods:
	// initialization:
	void initVariables();
	void initGraphicsSettings();
	void initWindow();
	void initKeys();
	void initEcosystem();
	void initStateData();
	void initStates();

	// other private methods:
	void updateDt();
	void update();
	void updateEvents();
	void render();
};
