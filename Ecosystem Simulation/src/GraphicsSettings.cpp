#include "stdafx.h"
#include "GraphicsSettings.h"

// constructor:
GraphicsSettings::GraphicsSettings()
{
	this->title = "DEFAULT TITLE";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->verticalSync = false;
	this->frameRateLimit = 144U;
	this->contextSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->position = std::make_pair(0, 0);
}

// methods:
void GraphicsSettings::saveToFile(const std::string& path)
{
	std::ofstream ofs(path);

	if (ofs.is_open())
	{
		ofs << this->title;
		ofs << this->resolution.width << ' ' << this->resolution.height;
		ofs << this->fullscreen;
		ofs << this->frameRateLimit;
		ofs << this->verticalSync;
		ofs << this->contextSettings.antialiasingLevel;
		ofs << this->position.first << ' ' << this->position.second;
	}
	else throw("ERROR::GRAPHICSSETTINGS::COULD NOT OPEN: " + path);

	ofs.close();
}

void GraphicsSettings::loadFromFile(const std::string& path)
{
	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::getline(ifs, this->title);
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->frameRateLimit;
		ifs >> this->verticalSync;
		ifs >> this->contextSettings.antialiasingLevel;
		ifs >> this->position.first >> this->position.second;
	}
	else throw("ERROR::GRAPHICSSETTINGS::COULD NOT OPEN: " + path);

	ifs.close();
}
