#pragma once

namespace gui
{
	float p2pX(float percent, const sf::VideoMode& resolution);
	float p2pY(float percent, const sf::VideoMode& resolution);
	unsigned calcCharSize(float per_mille, const sf::VideoMode& resolution);
}
