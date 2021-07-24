#pragma once

#include "Gui.h"
#include "EventsAccessor.h"

namespace gui
{
	class ImageButton
	{
	public:
		// constructor:
		ImageButton(
			const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths,
			const std::string& key_of_default_texture,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			int id = 0
		);

		// accessors:
		bool hasBeenClicked() const;

		bool isPressed() const;

		bool isHovered() const;

		const std::string& getCurrentTextureKey() const;

		const sf::Vector2f& getPosition() const;
		
		const sf::Vector2f& getSize() const;

		// mutators:
		void setTexture(const std::string& key);

		void setPosition(const sf::Vector2f& new_pos);

		// other public methods:
		void update(const sf::Vector2i& mouse_pos_window, const std::vector<sf::Event>& events);
		void render(sf::RenderTarget& target) const;

	private:
		std::unordered_map<std::string, sf::Texture> textures;

		sf::Sprite sprite;

		int id;

		bool hasBeenClickedSinceLastFrame;

		bool pressed;

		bool hovered;

		std::string currentTextureKey;

		// initialization:
		void initTextures(const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths);
		void initSprite(
			const std::string& key_of_default_texture,
			const sf::Vector2f& position,
			const sf::Vector2f& size
		);
	};
}
