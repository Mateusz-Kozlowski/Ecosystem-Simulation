#pragma once

namespace gui
{
	class FPS
	{
	public:
		static void init(
			float delay,
			const sf::Color& backgroundColor,
			unsigned charSize,
			const sf::Font& font,
			const sf::Color& textColor,
			const sf::VideoMode& resolution
		);

		static void update(float dt);
		static void render(sf::RenderTarget& target);

		// accessors:

		static bool isInitialized();

		static const unsigned long long& getTotalFramesCount();
		static float getTotalTimeElapsed();

		static float getDelay();

		static const sf::RectangleShape& getBackground();
		static sf::VideoMode& getResolution();

		// mutators:

		static void setDelay(float delay);

		static void setBackgroundColor(const sf::Color& color);
		static void setCharSize(unsigned charSize);
		static void setFont(const sf::Font& font);
		static void setTextColor(const sf::Color& color);
		static void setResolution(const sf::VideoMode& resolution);

	private:
		static void updateFrames();
		static void updateTime(float dt);
		static void updateFPS();
		static void updateText();
		static void updateBg();
		static void reset();

	private:
		static bool s_initialized;

		static unsigned long long s_totalFramesCount;
		static float s_totalTimeElapsed;

		static unsigned s_framesCount;
		static float s_timer;

		static float s_delay;

		static unsigned s_FPS;

		static sf::RectangleShape s_bg;
		static sf::Text s_text;
		static sf::VideoMode s_resolution;
	};
}
