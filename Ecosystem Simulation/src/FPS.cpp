#include "pch.h"
#include "FPS.h"

bool gui::FPS::s_initialized = false;

unsigned long long gui::FPS::s_totalFramesCount = 0U;
float gui::FPS::s_totalTimeElapsed = 0.0f;

unsigned gui::FPS::s_framesCount = 0U;
float gui::FPS::s_timer = 0.0f;

float gui::FPS::s_delay = 1.0f;

unsigned gui::FPS::s_FPS = 0U;

sf::Text gui::FPS::s_text;

sf::RectangleShape gui::FPS::s_bg;

sf::VideoMode gui::FPS::s_resolution = sf::VideoMode(1920U, 1080U);

void gui::FPS::init(
	float delay,
	const sf::Color& backgroundColor, 
	unsigned charSize, 
	const sf::Font& font, 
	const sf::Color& textColor,
	const sf::VideoMode& resolution)
{
	s_initialized = true;
	
	s_delay = delay;

	s_bg.setFillColor(backgroundColor);

	s_text.setCharacterSize(charSize);
	s_text.setFont(font);
	s_text.setFillColor(textColor);

	s_resolution = resolution;

	updateText();
	updateBg();
}

void gui::FPS::update(float dt)
{
	assert(s_initialized);

	updateFrames();
	updateTime(dt);

	if (s_timer >= s_delay)
	{
		updateFPS();
		updateText();
		updateBg();
		reset();
	}
}

void gui::FPS::render(sf::RenderTarget& target)
{
	assert(s_initialized);

	target.draw(s_bg);
	target.draw(s_text);
}

// accessors:

bool gui::FPS::isInitialized()
{
	return s_initialized;
}

const unsigned long long& gui::FPS::getTotalFramesCount()
{
	return s_totalFramesCount;
}

float gui::FPS::getTotalTimeElapsed()
{
	return s_totalTimeElapsed;
}

float gui::FPS::getDelay()
{
	return s_delay;
}

const sf::RectangleShape& gui::FPS::getBackground()
{
	return s_bg;
}

sf::VideoMode& gui::FPS::getResolution()
{
	return s_resolution;
}

// mutators:

void gui::FPS::setDelay(float delay)
{
	s_delay = delay;
}

void gui::FPS::setCharSize(unsigned charSize)
{
	s_text.setCharacterSize(charSize);
	updateText();
	updateBg();
}

void gui::FPS::setFont(const sf::Font& font)
{
	s_text.setFont(font);
	updateText();
	updateBg();
}

void gui::FPS::setBackgroundColor(const sf::Color& color)
{
	s_bg.setFillColor(color);
}

void gui::FPS::setTextColor(const sf::Color& color)
{
	s_text.setFillColor(color);
}

void gui::FPS::setResolution(const sf::VideoMode& resolution)
{
	s_resolution = resolution;
	updateText();
	updateBg();
}

// private methods:

void gui::FPS::updateFrames()
{
	s_totalFramesCount++;
	s_framesCount++;
}

void gui::FPS::updateTime(float dt)
{
	s_totalTimeElapsed += dt;
	s_timer += dt;
}

void gui::FPS::updateFPS()
{
	s_FPS = static_cast<unsigned>(s_framesCount / s_timer);
}

void gui::FPS::updateText()
{
	s_text.setString(std::to_string(s_FPS));

	const sf::FloatRect& textBounds = s_text.getLocalBounds();

	s_text.setOrigin(
		textBounds.left + textBounds.width / 2.0f, 
		textBounds.top + textBounds.height / 2.0f
	);

	s_text.setPosition(
		s_resolution.width - textBounds.width / 2.0f,
		textBounds.height / 2.0f
	);
}

void gui::FPS::updateBg()
{
	s_bg.setPosition(
		s_text.getPosition().x - s_text.getGlobalBounds().width / 2.0f,
		s_text.getPosition().y - s_text.getGlobalBounds().height / 2.0f
	);

	s_bg.setSize(
		sf::Vector2f(
			s_text.getGlobalBounds().width,
			s_text.getGlobalBounds().height
		)
	);
}

void gui::FPS::reset()
{
	s_framesCount = 0U;
	s_timer = 0.0f;
}
