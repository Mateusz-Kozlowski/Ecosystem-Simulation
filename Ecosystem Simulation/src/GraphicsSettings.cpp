#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings()
	: videoModes(sf::VideoMode::getFullscreenModes())
	, resolution(sf::VideoMode::getDesktopMode())
	, title("DEFAULT TITLE")
	, contextSettings()
	, fullscreen(false)
	, verticalSync(false)
	, frameRateLimit(144U)
	, position(0, 0)
{
	contextSettings.antialiasingLevel = 0;
}

void GraphicsSettings::saveToFile(const std::string& path)
{
	std::ofstream ofs(path);

	if (ofs.is_open())
	{
		ofs << title;
		ofs << resolution.width << ' ' << resolution.height;
		ofs << fullscreen;
		ofs << frameRateLimit;
		ofs << verticalSync;
		ofs << contextSettings.antialiasingLevel;
		ofs << position.first << ' ' << position.second;
	}
	else
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::GraphicsSettings::saveToFile(const std::string&)::"
			<< "could not open "
			<< path << '\n'
		);
	}

	ofs.close();
}

void GraphicsSettings::loadFromFile(const std::string& path)
{
	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::getline(ifs, title);
		ifs >> resolution.width >> resolution.height;
		ifs >> fullscreen;
		ifs >> frameRateLimit;
		ifs >> verticalSync;
		ifs >> contextSettings.antialiasingLevel;
		ifs >> position.first >> position.second;
	}
	else
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::GraphicsSettings::loadFromFile(const std::string&)::"
			<< "could not open "
			<< path << '\n'
		);
	}

	ifs.close();
}
