#include "pch.h"
#include "FPSpreview.h"

bool gui::FPSpreview::s_initialized = false;

unsigned long long gui::FPSpreview::s_totalFramesCount = 0U;
float gui::FPSpreview::s_totalTimeElapsed = 0.0f;

unsigned gui::FPSpreview::s_framesCount = 0U;
float gui::FPSpreview::s_timer = 0.0f;

float gui::FPSpreview::s_delay = 1.0f;

unsigned gui::FPSpreview::s_FPSpreview = 0U;

sf::Text gui::FPSpreview::s_text;

sf::RectangleShape gui::FPSpreview::s_bg;

sf::VideoMode gui::FPSpreview::s_resolution = sf::VideoMode(1920U, 1080U);

// public methods:

void gui::FPSpreview::init(
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

void gui::FPSpreview::update(float dt)
{
	assert(s_initialized);

	updateFrames();
	updateTime(dt);

	if (s_timer >= s_delay)
	{
		updateFPSpreview();
		updateText();
		updateBg();
		reset();
	}
}

void gui::FPSpreview::render(sf::RenderTarget& target)
{
	assert(s_initialized);

	target.draw(s_bg);
	target.draw(s_text);
}

// accessors:

bool gui::FPSpreview::isInitialized()
{
	return s_initialized;
}

const unsigned long long& gui::FPSpreview::getTotalFramesCount()
{
	return s_totalFramesCount;
}

float gui::FPSpreview::getTotalTimeElapsed()
{
	return s_totalTimeElapsed;
}

float gui::FPSpreview::getDelay()
{
	return s_delay;
}

const sf::RectangleShape& gui::FPSpreview::getBackground()
{
	return s_bg;
}

sf::VideoMode& gui::FPSpreview::getResolution()
{
	return s_resolution;
}

// mutators:

void gui::FPSpreview::setDelay(float delay)
{
	s_delay = delay;
}

void gui::FPSpreview::setCharSize(unsigned charSize)
{
	s_text.setCharacterSize(charSize);
	updateText();
	updateBg();
}

void gui::FPSpreview::setFont(const sf::Font& font)
{
	s_text.setFont(font);
	updateText();
	updateBg();
}

void gui::FPSpreview::setBackgroundColor(const sf::Color& color)
{
	s_bg.setFillColor(color);
}

void gui::FPSpreview::setTextColor(const sf::Color& color)
{
	s_text.setFillColor(color);
}

void gui::FPSpreview::setResolution(const sf::VideoMode& resolution)
{
	s_resolution = resolution;
	updateText();
	updateBg();
}

// private methods:

void gui::FPSpreview::updateFrames()
{
	s_totalFramesCount++;
	s_framesCount++;
}

void gui::FPSpreview::updateTime(float dt)
{
	s_totalTimeElapsed += dt;
	s_timer += dt;
}

void gui::FPSpreview::updateFPSpreview()
{
	s_FPSpreview = static_cast<unsigned>(s_framesCount / s_timer);
}

void gui::FPSpreview::updateText()
{
	s_text.setString(std::to_string(s_FPSpreview));

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

void gui::FPSpreview::updateBg()
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

void gui::FPSpreview::reset()
{
	s_framesCount = 0U;
	s_timer = 0.0f;
}
