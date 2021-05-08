#pragma once

#include "MainMenuState.h"

class App
{
public:
	// constructor/destructor:
	App();
	~App();
	
	// core:
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
