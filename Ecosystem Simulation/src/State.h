#pragma once

#include "GraphicsSettings.h"
#include "Ecosystem.h"

class State;

class StateData
{
public:
	// constructor:
	StateData()
	{
		this->gfxSettings = nullptr;
		this->window = nullptr;
		this->supportedKeys = nullptr;
		this->states = nullptr;
		this->ecosystem = nullptr;
		this->events = nullptr;
	}

	GraphicsSettings* gfxSettings;
	sf::RenderWindow* window;
	std::unordered_map<std::string, int>* supportedKeys;
	std::stack<State*>* states;
	Ecosystem* ecosystem;
	std::vector<sf::Event>* events;
};

class State
{
public:
	// constructor/destructor:
	State(StateData* state_data);
	virtual ~State();

	// accessors:
	bool getQuit() const;

	// mutators:
	void endState();

	virtual void freeze();

	virtual void update(float dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

protected:
	StateData* stateData;

	std::unordered_map<std::string, int> keybinds;

	bool quit;
	
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// protected methods:
	void updateMousePositions(sf::View* view = nullptr);
	
	virtual void initKeybinds() = 0;
	virtual void updateInput() = 0;
};
