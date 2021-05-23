#pragma once

#include "Gui.h"

namespace gui
{
	class TextureButton
	{
	public:
		// constructor:
		TextureButton(
			const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths,
			const std::string& key_of_default_texture,
			float x, float y,
			float width, float height,
			int id = 0
		);

		// accessors:
		bool hasBeenClicked() const;

		const std::string& getCurrentTextureKey() const;

		const sf::Vector2f& getPosition() const;

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

		std::string currentTextureKey;

		// initialization:
		void initTextures(const std::vector<std::pair<std::string, std::string>>& textures_keys_and_paths);
		void initSprite(const std::string& key_of_default_texture, float x, float y, float width, float height);
	};
}
