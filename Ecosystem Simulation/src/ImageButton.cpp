#include "ImageButton.h"

gui::ImageButton::ImageButton(
	const std::vector<StringsPair>& texturesKeysAndPaths,
	const std::string& keyOfDefaultTexture,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	int id)
	: m_textures()
	, m_sprite()
	, m_id(id)
	, m_hasBeenClickedSinceLastFrame(false)
	, m_pressed(false)
	, m_hovered(false)
	, m_currentTextureKey(keyOfDefaultTexture)
{
	initTextures(texturesKeysAndPaths);
	initSprite(keyOfDefaultTexture, position, size);
}

void gui::ImageButton::update(
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	m_hasBeenClickedSinceLastFrame = false;
	m_pressed = false;
	m_hovered = false;

	// temporary variables:
	float posX = m_sprite.getPosition().x;
	float posY = m_sprite.getPosition().y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		std::cout << mousePos.x << ' ' << mousePos.y << '\n';
		std::cout << posX << ' ' << posY << '\n';
	}

	if (posX < mousePos.x 
		&& mousePos.x < posX + m_sprite.getGlobalBounds().width)
	{
		if (posY < mousePos.y 
			&& mousePos.y < posY + m_sprite.getGlobalBounds().height)
		{
			m_hovered = true;

			// check if has been clicked:

			bool hasEventOccured = EventsAccessor::hasEventOccured(
				sf::Event::MouseButtonPressed,
				events
			);

			if (hasEventOccured)
			{
				m_hasBeenClickedSinceLastFrame = true;
			}

			// check if a button is pressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				m_pressed = true;
			}
		}
	}
}

void gui::ImageButton::render(sf::RenderTarget& target) const
{
	target.draw(m_sprite);
}

// accessors:

bool gui::ImageButton::hasBeenClicked() const
{
	return m_hasBeenClickedSinceLastFrame;
}

bool gui::ImageButton::isPressed() const
{
	return m_pressed;
}

bool gui::ImageButton::isHovered() const
{
	return m_hovered;
}

const std::string& gui::ImageButton::getCurrentTextureKey() const
{
	return m_currentTextureKey;
}

const sf::Vector2f& gui::ImageButton::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::Vector2f& gui::ImageButton::getSize() const
{
	return sf::Vector2f(
		m_sprite.getLocalBounds().width, 
		m_sprite.getLocalBounds().height
	);
}

// mutators:

void gui::ImageButton::setTexture(const std::string& key)
{
	m_sprite.setTexture(m_textures[key]);

	m_currentTextureKey = key;
}

void gui::ImageButton::setPosition(const sf::Vector2f& newPos)
{
	m_sprite.setPosition(newPos);
}

void gui::ImageButton::setSize(const sf::Vector2f& size)
{
	setSpriteSize(size);
}

// private methods:

void gui::ImageButton::initTextures(
	const std::vector<StringsPair>& texturesKeysAndPaths)
{
	for (const auto& it : texturesKeysAndPaths)
	{
		if (!m_textures[it.first].loadFromFile(it.second))
		{
			throw std::runtime_error(
				Blueberry::Formatter()
				<< "Error::gui::ImageButton::initTextures"
				<< "(const std::vector<std::pair<std::string, std::string>>&::"
				<< "cannot load the following texture: "
				<< it.second
				<< '\n'
			);
		}
	}
}

void gui::ImageButton::initSprite(
	const std::string& keyOfDefaultTexture,
	const sf::Vector2f& position,
	const sf::Vector2f& size)
{
	m_sprite.setTexture(m_textures[keyOfDefaultTexture]);
	m_sprite.setPosition(position.x, position.y);

	setSpriteSize(size);
}

void gui::ImageButton::setSpriteSize(const sf::Vector2f& size)
{
	m_sprite.scale(
		size.x / m_sprite.getGlobalBounds().width,
		size.y / m_sprite.getGlobalBounds().height
	);
}
