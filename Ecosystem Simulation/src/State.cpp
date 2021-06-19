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

bool State::isPaused() const
{
	return this->paused;
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
void State::updateMousePositions(const sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->stateData->window);

	if (view)
	{
		sf::View temp = this->stateData->window->getView();

		this->stateData->window->setView(*view);

		this->mousePosView = this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window));

		this->stateData->window->setView(temp);
	}
	else
		this->mousePosView = this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window));
}
