#include "stdafx.h"
#include "TextureButton.h"

using namespace gui;

TextureButton::TextureButton(
	const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths,
	const std::string& key_of_default_texture,
	const sf::Vector2f& pos,
	const sf::Vector2f& size,
	int id)
	: id(id), hasBeenClickedSinceLastFrame(false), hovered(false), currentTextureKey(key_of_default_texture)
{
	this->initTextures(textures_keys_and_paths);
	this->initSprite(key_of_default_texture, pos, size);
}

// accessors:
bool gui::TextureButton::hasBeenClicked() const
{
	return this->hasBeenClickedSinceLastFrame;
}

bool gui::TextureButton::isPressed() const
{
	return this->pressed;
}

bool gui::TextureButton::isHovered() const
{
	return this->hovered;
}

const std::string& gui::TextureButton::getCurrentTextureKey() const
{
	return this->currentTextureKey;
}

const sf::Vector2f& gui::TextureButton::getPosition() const
{
	return this->sprite.getPosition();
}

const sf::Vector2f& gui::TextureButton::getSize() const
{
	return sf::Vector2f(this->sprite.getLocalBounds().width, this->sprite.getLocalBounds().height);
}

// mutators:
void TextureButton::setTexture(const std::string& key)
{
	this->sprite.setTexture(this->textures[key]);

	this->currentTextureKey = key;
}

void gui::TextureButton::setPosition(const sf::Vector2f& new_pos)
{
	this->sprite.setPosition(new_pos);
}

// other public methods:
void gui::TextureButton::update(const sf::Vector2i& mouse_pos_window, const std::vector<sf::Event>& events)
{
	this->hasBeenClickedSinceLastFrame = false;
	this->pressed = false;
	this->hovered = false;

	// temporary variables:
	float posX = this->sprite.getPosition().x;
	float posY = this->sprite.getPosition().y;

	if (posX < mouse_pos_window.x && mouse_pos_window.x < posX + this->sprite.getGlobalBounds().width)
	{
		if (posY < mouse_pos_window.y && mouse_pos_window.y < posY + this->sprite.getGlobalBounds().height)
		{
			this->hovered = true;

			// check if has been clicked:
			for (const auto& event : events)
			{
				if (event.type == sf::Event::MouseButtonPressed)
				{
					this->hasBeenClickedSinceLastFrame = true;
					break;
				}
			}

			// check if a button is pressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) this->pressed = true;
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

void gui::TextureButton::initSprite(
	const std::string& key_of_default_texture, 
	const sf::Vector2f& pos, 
	const sf::Vector2f& size)
{
	this->sprite.setTexture(this->textures[key_of_default_texture]);
	this->sprite.setPosition(pos.x, pos.y);

	this->sprite.setScale(
		size.x / this->sprite.getGlobalBounds().width,
		size.y / this->sprite.getGlobalBounds().height
	);
}
