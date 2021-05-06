#include "stdafx.h"
#include "MiniMap.h"

MiniMap::MiniMap(sf::Vector2u world_size, sf::Vector2u mini_map_size, sf::Vector2u mini_map_pos)
{
	/*
	this->view.setSize(sf::Vector2f(world_size.x / 2U, world_size.y / 2U));
	this->view.setCenter(sf::Vector2f(world_size.x / 2U, world_size.y / 2U));
	this->view.setViewport(sf::FloatRect(sf::Vector2f(mini_map_pos), sf::Vector2f(mini_map_size)));
	*/

	this->view = sf::View(sf::FloatRect(0, 0, 3840, 2160));
	//this->view.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));

	this->view.setSize(3840, 2160);
}

void MiniMap::render(sf::RenderTexture& render_texture)
{
	//render_texture.setView(this->view);

	this->view.setViewport(sf::FloatRect(0.75f, 0.75f, 0.25f, 0.25f));

	sf::Sprite temp(render_texture.getTexture());

	temp.scale(0.25, 0.25);

	temp.setPosition(0, 0);

	render_texture.draw(temp);
}
