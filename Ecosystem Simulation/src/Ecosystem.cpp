#include "stdafx.h"
#include "Ecosystem.h"

std::string Ecosystem::configFileTemplatePath = "resources/template files/ecosystem initializator.ini";
std::string Ecosystem::configFileName = "ecosystem initializator.ini";

// constructor/destructor:
Ecosystem::Ecosystem()
	: directoryPath("ECOSYSTEM IS NOT INITIALIZED"), borderThickness(0U)
{
	
}

Ecosystem::~Ecosystem()
{
	for (auto& individual : this->individuals) delete individual;
}

// accessors:
const sf::Vector2f& Ecosystem::getWorldSize() const
{
	return this->worldSize;
}

const std::string& Ecosystem::getDirectoryPath() const
{
	return this->directoryPath;
}

unsigned Ecosystem::getBorderThickness() const
{
	return this->borderThickness;
}

// public initialization methods:
void Ecosystem::create(const std::string& folder_path)
{
	std::ifstream file(folder_path + '/' + Ecosystem::configFileName);

	if (!file.is_open()) std::cerr << "ERROR::ECOSYSTEM::CANNOT OPEN FILE: " + folder_path + '/' + Ecosystem::configFileName;

	std::string temp;
	unsigned individualsCount = 0U;

	file >> temp >> this->worldSize.x;
	file >> temp >> this->worldSize.y;
	file >> temp >> this->borderThickness;
	file >> temp >> individualsCount;

	file.close();

	this->border.setFillColor(sf::Color(32, 32, 32));
	this->border.setSize(this->worldSize);

	this->background.setFillColor(sf::Color::Black);
	this->background.setSize(
		sf::Vector2f(
			this->worldSize.x - 2.f * this->borderThickness,
			this->worldSize.y - 2.f * this->borderThickness
		)
	);
	this->background.setPosition(this->borderThickness, this->borderThickness);

	for (int i = 0; i < individualsCount; i++) this->individuals.push_back(new Individual(i * 128, i * 100));

	this->directoryPath = folder_path;

	std::cout << "Ecosystem created\n";
}

void Ecosystem::loadFromFolder(const std::string& folder_path)
{
	std::cout << "LOADING FROM FOLDER!\n";
}

// other public methods:
void Ecosystem::update(float dt)
{
	for (auto& individual : this->individuals) individual->update(dt);
}

void Ecosystem::render(sf::RenderTarget& target)
{
	target.draw(this->border);
	target.draw(this->background);

	for (auto& individual : this->individuals) individual->render(target);
}

// static methods:
void Ecosystem::createConfigFile(const std::string& directory_path)
{
	std::ifstream src(Ecosystem::configFileTemplatePath, std::ios::binary);
	std::ofstream dst(directory_path + "/" + Ecosystem::configFileName, std::ios::binary);

	if (!src.is_open()) throw("ERROR::CANNOT OPEN A FILE: " + Ecosystem::configFileTemplatePath);
	if (!dst.is_open()) throw("ERROR::CANNOT OPEN A FILE: " + directory_path + '/' + Ecosystem::configFileName);

	dst << src.rdbuf();

	dst.close();
}
