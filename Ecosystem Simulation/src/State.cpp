#include "pch.h"
#include "State.h"

State::State(StateData* stateData)
	: m_stateData(stateData)
	, m_keybinds()
	, m_quit(false)
	, m_mousePosScreen()
	, m_mousePosWindow()
	, m_mousePosView()
{
	
}

State::~State()
{

}

// accessors:

bool State::getQuit() const
{
	return m_quit;
}

// mutators:

void State::endState()
{
	m_quit = true;
}

void State::freeze()
{
	std::cerr << "FREEZING IS NOT DEFINED YET!\n";
}

// protected methods:

void State::updateMousePositions(const sf::View* view)
{
	m_mousePosScreen = sf::Mouse::getPosition();
	m_mousePosWindow = sf::Mouse::getPosition(*m_stateData->m_window);

	if (view)
	{
		sf::View temp = m_stateData->m_window->getView();

		m_stateData->m_window->setView(*view);

		m_mousePosView = m_stateData->m_window->mapPixelToCoords(
			sf::Mouse::getPosition(
				*m_stateData->m_window
			)
		);

		m_stateData->m_window->setView(temp);
	}
	else
	{
		m_mousePosView = m_stateData->m_window->mapPixelToCoords(
			sf::Mouse::getPosition(
				*m_stateData->m_window
			)
		);
	}
}
