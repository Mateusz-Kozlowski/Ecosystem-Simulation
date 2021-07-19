#include "stdafx.h"
#include "Fruit.h"

Fruit::Fruit(
	float energy,
	const sf::Vector2f& position,
	float radius,
	const sf::Color& color)
	: energy(energy)
{
	this->initShape(position, radius, color);
}

Fruit::Fruit(const std::string& file_path)
{
	this->loadFromFile(file_path);
}

// public methods:
void Fruit::saveToFile(const std::string& file_path) const
{
	std::ofstream ofs(file_path);

	if (!ofs.is_open())
	{
		std::cerr << "ERROR::Fruit::saveToFile::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	ofs << this->energy << '\n';
	ofs << this->shape.getPosition().x << ' ' << this->shape.getPosition().y << '\n';
	ofs << this->shape.getRadius() << '\n';
	ofs << static_cast<int>(this->shape.getFillColor().r) << ' ';
	ofs << static_cast<int>(this->shape.getFillColor().g) << ' ';
	ofs << static_cast<int>(this->shape.getFillColor().b) << ' ';
	ofs << static_cast<int>(this->shape.getFillColor().a);

	ofs.close();
}

void Fruit::loadFromFile(const std::string& file_path)
{
	std::ifstream ifs(file_path);

	if (!ifs.is_open())
	{
		std::cerr << "ERROR::Fruit::saveToFile::CANNOT OPEN: " << file_path << '\n';
		exit(-1);
	}

	sf::Vector2f position;
	float radius;
	unsigned red, green, blue, alfa;

	ifs >> this->energy;
	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> red >> green >> blue >> alfa;

	this->initShape(position, radius, sf::Color(red, green, blue, alfa));

	ifs.close();
}

void Fruit::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
}

bool Fruit::isCoveredByMouse(const sf::Vector2f& mouse_pos_view) const
{
	float x = this->shape.getPosition().x - mouse_pos_view.x;
	float y = this->shape.getPosition().y - mouse_pos_view.y;

	float distance = sqrt(pow(x, 2) + pow(y, 2));

	return this->getRadius() >= distance;
}

// accessors:
const sf::Vector2f& Fruit::getPosition() const
{
	return this->shape.getPosition();
}

float Fruit::getRadius() const
{
	return this->shape.getRadius();
}

float Fruit::getEnergy() const
{
	return this->energy;
}

const sf::Color& Fruit::getColor() const
{
	return this->shape.getFillColor();
}

// mutators:
void Fruit::setPosition(float x, float y)
{
	this->shape.setPosition(x, y);
}

void Fruit::setPosition(const sf::Vector2f& new_position)
{
	this->shape.setPosition(new_position);
}

void Fruit::setRandomPosition(const sf::Vector2f& world_size, float borders_thickness)
{
	std::pair<unsigned, unsigned> rangeX = {
		borders_thickness + this->shape.getRadius(),
		world_size.x - borders_thickness - this->shape.getRadius()
	};
	
	std::pair<unsigned, unsigned> rangeY = {
		borders_thickness + this->shape.getRadius(),
		world_size.y - borders_thickness - this->shape.getRadius()
	};

	float x = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(rangeX);
	float y = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(rangeY);

	this->shape.setPosition(x, y);
}

void Fruit::setRadius(float radius)
{
	this->shape.setRadius(radius);
}

void Fruit::setEnergy(float energy)
{
	this->energy = energy;
}

void Fruit::increaseEnergy(float increase)
{
	this->energy += increase;
}

void Fruit::setColor(const sf::Color& color)
{
	this->shape.setFillColor(color);
}

// private methods:

// initialization:
void Fruit::initShape(const sf::Vector2f& position, float radius, const sf::Color& color)
{
	this->shape.setPosition(position);
	this->shape.setRadius(radius);
	this->shape.setOrigin(radius, radius);
	this->shape.setFillColor(color);
}
