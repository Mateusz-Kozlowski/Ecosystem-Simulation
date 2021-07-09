#include "stdafx.h"
#include "Fruit.h"

// constructors:
Fruit::Fruit()
	: energy(0.0f)
{

}

Fruit::Fruit(
	float energy,
	const sf::Vector2f& position,
	float radius,
	const sf::Color& color)
	: energy(energy)
{
	this->initShape(position, radius, color);
}

// public methods:
void Fruit::saveToFile(const std::string& path) const
{
	std::ofstream ofs(path);

	if (!ofs.is_open())
	{
		std::cerr << "ERROR::Fruit::saveToFile::CANNOT OPEN: " << path << '\n';
		exit(-1);
	}

	ofs << this->energy << '\n';
	ofs << this->shape.getPosition().x << ' ' << this->shape.getPosition().y << '\n';
	ofs << this->shape.getRadius() << '\n';
	ofs << static_cast<int>(this->shape.getFillColor().r);
	ofs << static_cast<int>(this->shape.getFillColor().g);
	ofs << static_cast<int>(this->shape.getFillColor().b);
	ofs << static_cast<int>(this->shape.getFillColor().a);

	ofs.close();
}

void Fruit::loadFromFile(const std::string& path)
{
	std::ifstream ifs(path);

	if (!ifs.is_open())
	{
		std::cerr << "ERROR::Fruit::saveToFile::CANNOT OPEN: " << path << '\n';
		exit(-1);
	}

	sf::Vector2f position;
	float radius;
	sf::Color color;

	ifs >> this->energy;
	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> color.r >> color.g >> color.b >> color.a;

	this->initShape(position, radius, color);

	ifs.close();
}

void Fruit::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
}

bool Fruit::isCovered(const sf::Vector2f& mouse_pos_view) const
{
	float a = this->shape.getPosition().x - mouse_pos_view.x;
	float b = this->shape.getPosition().y - mouse_pos_view.y;

	float distance = sqrt(pow(a, 2) + pow(b, 2));

	return this->getRadius() >= distance;
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
	std::pair<unsigned, unsigned> px = { borders_thickness, world_size.x - borders_thickness };
	std::pair<unsigned, unsigned> py = { borders_thickness, world_size.y - borders_thickness };

	float x = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(px);
	float y = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(py);

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

void Fruit::setColor(const sf::Color& color)
{
	this->shape.setFillColor(color);
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

// private methods:

// initialization:
void Fruit::initShape(const sf::Vector2f& position, float radius, const sf::Color& color)
{
	this->shape.setPosition(position);
	this->shape.setRadius(radius);
	this->shape.setOrigin(radius, radius);
	this->shape.setFillColor(color);
}
