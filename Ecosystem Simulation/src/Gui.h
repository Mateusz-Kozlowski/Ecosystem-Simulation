#pragma once

enum class ButtonState { BTN_IDLE = 0, BTN_HOVERED, BTN_PRESSED };

namespace gui
{
	float p2pX(float perc, const sf::VideoMode& vm);
	float p2pY(float perc, const sf::VideoMode& vm);
	unsigned calcCharSize(const sf::VideoMode& vm, unsigned height_per_mille);

	class Button
	{
	public:
		Button(
			float posX, float posY,
			float width, float height,
			const sf::Font& font, const std::string& text, int charSize,
			sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor,
			sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlinePressedColor,
			sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textPressedColor,
			float outlineThickness = 1.f, int id = 0
		);

		bool isClicked() const;

		const std::string& getText() const;
		int getId() const;
		ButtonState getState() const;

		void setText(const std::string& text);
		void setId(int id);
		void setClickBlockade(bool blockade);
		void setIdle();
		void setHovered();

		void update(sf::Vector2i mousePosWindow);
		void render(sf::RenderTarget& target);

	private:
		bool clickBlockade;
		bool clicked;
		ButtonState state;
		unsigned short id;

		sf::RectangleShape rect;

		std::shared_ptr<sf::Font> font;
		sf::Text text;

		sf::Color idleColor, hoverColor, pressedColor;
		sf::Color outlineIdleColor, outlineHoverColor, outlinePressedColor;
		sf::Color textIdleColor, textHoverColor, textPressedColor;
	};

	class Modifier
	{
	public:
		Modifier(
			float posX, float posY,
			float textureScale,
			float maxValue, bool maxOnLeft,
			const std::string& axis_idle_path, const std::string& handle_idle_path,
			const std::string& axis_hover_path, const std::string& handle_hover_path,
			const std::string& axis_pressed_path, const std::string& handle_pressed_path
		);

		float getValue() const;
		float getMaxValue() const;

		void setValue(float value);
		void setMinimizeToZero(bool minimizeToZero);

		void update(sf::Vector2i mousePosWindow);
		void render(sf::RenderTarget& target);

	private:
		std::unordered_map<std::string, sf::Texture> axisTextures;
		std::unordered_map<std::string, sf::Texture> handleTextures;

		std::unordered_map<std::string, sf::Sprite> axes;
		std::unordered_map<std::string, sf::Sprite> handles;

		std::string state;

		bool maxOnLeft;
		bool minimizeToZero;

		float maxValue;
		float value;
	};
}
