#pragma once

class GraphicsSettings
{
public:
	GraphicsSettings();
	
	void saveToFile(const std::string& path);
	void loadFromFile(const std::string& path);
	
	std::vector<sf::VideoMode> videoModes;
	sf::VideoMode resolution;
	std::string title;
	sf::ContextSettings contextSettings;
	bool fullscreen;
	bool verticalSync;
	unsigned frameRateLimit;
	std::pair<int, int> position;
};
