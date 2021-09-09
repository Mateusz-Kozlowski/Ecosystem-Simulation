#pragma once

#include "Gui.h"
#include "EventsAccessor.h"

namespace gui
{
	class ImageButton
	{
	private:
		typedef std::pair<std::string, std::string> StringsPair;

	public:
		ImageButton(
			const std::vector<StringsPair>& texturesKeysAndPaths,
			const std::string& keyOfDefaultTexture,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			int id = 0
		);

		void update(
			const sf::Vector2f& mousePos, 
			const std::vector<sf::Event>& events
		);
		void render(sf::RenderTarget& target) const;

		// accessors:

		bool hasBeenClicked() const;
		bool isPressed() const;
		bool isHovered() const;

		const std::string& getCurrentTextureKey() const;

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getSize() const;

		// mutators:

		void setTexture(const std::string& key);
		void setPosition(const sf::Vector2f& newPos);
		void setSize(const sf::Vector2f& size);

	private:
		void initTextures(
			const std::vector<StringsPair>& texturesKeysAndPaths
		);
		void initSprite(
			const std::string& keyOfDefaultTexture,
			const sf::Vector2f& position,
			const sf::Vector2f& size
		);

		void setSpriteSize(const sf::Vector2f& size);

	private:
		std::unordered_map<std::string, sf::Texture> m_textures;

		sf::Sprite m_sprite;

		int m_id;

		bool m_hasBeenClickedSinceLastFrame;
		bool m_pressed;
		bool m_hovered;

		std::string m_currentTextureKey;
	};
}
