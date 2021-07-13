#pragma once

#include "GraphicsSettings.h"
#include "Ecosystem.h"

class State;

class StateData
{
public:
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
	State(StateData* state_data);
	virtual ~State();

	// public methods:
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

	// accessors:
	bool getQuit() const;

	// mutators:
	void endState();

	virtual void freeze();
	
protected:
	StateData* stateData;

	std::unordered_map<std::string, int> keybinds;

	bool quit;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// protected methods:
	void updateMousePositions(const sf::View* view = nullptr);
	
	virtual void initKeybinds() = 0;
	virtual void updateInput() = 0;
};
