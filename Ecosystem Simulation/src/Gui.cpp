#include "stdafx.h"
#include "Gui.h"

float gui::p2pX(float percent, const sf::VideoMode& resolution)
{
	return static_cast<float>(resolution.width) * (percent / 100.0f);
}

float gui::p2pY(float percent, const sf::VideoMode& resolution)
{
	return static_cast<float>(resolution.height) * (percent / 100.0f);
}

unsigned gui::calcCharSize(float per_mille, const sf::VideoMode& resolution)
{
	return static_cast<unsigned>(resolution.height * (per_mille / 1000.0f));
}
