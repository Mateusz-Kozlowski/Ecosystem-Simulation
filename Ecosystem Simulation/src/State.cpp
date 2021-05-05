#include "stdafx.h"
#include "State.h"

// constructor/destructor:
State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->quit = false;
}

State::~State()
{
}

// accessors:
bool State::getQuit() const
{
	return this->quit;
}

// mutators:
void State::endState()
{
	this->quit = true;
}

void State::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

// protected methods:
void State::updateMousePositions(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->stateData->window);

	if (view)
		this->stateData->window->setView(*view);

	this->mousePosView = this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window));

	this->stateData->window->setView(this->stateData->window->getDefaultView());
}