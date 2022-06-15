#pragma once

#include "Libraries.h"

namespace gui
{
	float p2pX(float percent, const sf::VideoMode& resolution);
	float p2pY(float percent, const sf::VideoMode& resolution);
	unsigned calcCharSize(float perMille, const sf::VideoMode& resolution);
}
