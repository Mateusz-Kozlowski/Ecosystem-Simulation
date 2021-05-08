#pragma once

namespace gui
{
	float p2pX(float perc, const sf::VideoMode& vm);
	float p2pY(float perc, const sf::VideoMode& vm);
	unsigned calcCharSize(const sf::VideoMode& vm, unsigned height_per_mille);
}
