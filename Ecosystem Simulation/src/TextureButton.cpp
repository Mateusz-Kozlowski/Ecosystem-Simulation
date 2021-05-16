#include "stdafx.h"
#include "TextureButton.h"

using namespace gui;

TextureButton::TextureButton(
	const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths,
	const std::string& key_of_default_texture,
	float x, float y,
	float width, float height,
	int id)
	: id(id), hasBeenClickedSinceLastFrame(false), currentTextureKey(key_of_default_texture)
{
	this->initTextures(textures_keys_and_paths);
	this->initSprite(key_of_default_texture, x, y, width, height);
}

// accessors:
bool gui::TextureButton::hasBeenClicked() const
{
	return this->hasBeenClickedSinceLastFrame;
}

const std::string& gui::TextureButton::getCurrentTextureKey() const
{
	return this->currentTextureKey;
}

// mutators:
void TextureButton::setTexture(const std::string& key)
{
	this->sprite.setTexture(this->textures[key]);

	this->sprite.scale(
		2.f, 2.f
	);

	this->currentTextureKey = key;
}

// other public methods:
void gui::TextureButton::update(const sf::Vector2i& mouse_pos_window, const std::vector<sf::Event>& events)
{
	this->hasBeenClickedSinceLastFrame = false;

	for (const auto& event : events)
	{
		if (event.type == sf::Event::MouseButtonReleased)
		{
			float posX = this->sprite.getPosition().x;
			float posY = this->sprite.getPosition().y;

			if (posX < mouse_pos_window.x && mouse_pos_window.x < posX + this->sprite.getGlobalBounds().width)
			{
				if (posY < mouse_pos_window.y && mouse_pos_window.y < posY + this->sprite.getGlobalBounds().height)
				{
					this->hasBeenClickedSinceLastFrame = true;
				}
			}

			return;
		}
	}
}

void TextureButton::render(sf::RenderTarget& target) const
{
	target.draw(this->sprite);
}

// private methods:
// initialization:
void gui::TextureButton::initTextures(const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths)
{
	for (const auto& it : textures_keys_and_paths)
	{
		if (!this->textures[it.first].loadFromFile(it.second))
			throw("ERROR::TEXTUREBUTTON::CANNOT LOAD FROM FILE: " + it.second);
	}
}

void gui::TextureButton::initSprite(const std::string& key_of_default_texture, float x, float y, float width, float height)
{
	this->sprite.setTexture(this->textures[key_of_default_texture]);
	this->sprite.setPosition(x, y);

	this->sprite.setScale(
		width / this->sprite.getGlobalBounds().width,
		height / this->sprite.getGlobalBounds().height
	);
}
