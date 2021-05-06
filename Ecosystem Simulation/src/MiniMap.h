#pragma once

class MiniMap
{
public:
	// constructor/destructor:
	MiniMap(sf::Vector2u world_size, sf::Vector2u mini_map_size, sf::Vector2u mini_map_pos);

	// public methods:
	void update() {}
	void render(sf::RenderTexture& render_texture);

private:
	sf::View view;

};
