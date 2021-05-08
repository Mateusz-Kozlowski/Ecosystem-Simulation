#include "stdafx.h"
#include "Gui.h"

float gui::p2pX(float perc, const sf::VideoMode& resolution)
{
	return std::floor(static_cast<float>(resolution.width) * (perc / 100.f));
}

float gui::p2pY(float perc, const sf::VideoMode& resolution)
{
	return std::floor(static_cast<float>(resolution.height) * (perc / 100.f));
}

unsigned gui::calcCharSize(const sf::VideoMode& resolution, unsigned height_per_mille)
{
	return static_cast<unsigned>(resolution.height * height_per_mille / 1000.f);
}
