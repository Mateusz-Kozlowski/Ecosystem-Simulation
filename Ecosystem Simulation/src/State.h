#pragma once

#include "GraphicsSettings.h"
#include "Ecosystem.h"

class State;

class StateData
{
public:
	GraphicsSettings* m_gfxSettings;
	sf::RenderWindow* m_window;
	std::unordered_map<std::string, int>* m_supportedKeys;
	std::stack<std::unique_ptr<State>>* m_states;
	Ecosystem* m_ecosystem;
	std::vector<sf::Event>* m_events;
};

class State
{
public:
	State(StateData* stateData);
	virtual ~State();
	
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

	// accessors:

	bool getQuit() const;

	// mutators:
	
	void endState();

	virtual void freeze();
	
protected:
	// initialization:

	virtual void initKeybinds() = 0;
	
	// utils:

	virtual void updateMousePositions(const sf::View* view = nullptr);
	virtual void updateInput() = 0;

protected:
	StateData* m_stateData;

	std::unordered_map<std::string, int> m_keybinds;

	bool m_quit;

	sf::Vector2i m_mousePosScreen;
	sf::Vector2i m_mousePosWindow;
	sf::Vector2f m_mousePosView;
};
