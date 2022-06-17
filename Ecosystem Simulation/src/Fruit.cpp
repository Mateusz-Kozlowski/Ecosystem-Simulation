#include "Fruit.h"

Fruit::Fruit(
	unsigned energy,
	const sf::Vector2f& position,
	float radius,
	const sf::Color& color)
	: m_energy(energy)
	, m_shape()
{
	initShape(position, radius, color);
}

Fruit::Fruit(const char* filePath)
	: m_energy(0U)
	, m_shape()
{
	loadFromFile(filePath);
}

void Fruit::saveToFile(const char* filePath) const
{
	std::filesystem::path dirPath = std::filesystem::path(filePath).remove_filename();
	
	if (!dirPath.empty())
	{
		std::filesystem::create_directories(dirPath);
	}

	std::ofstream ofs(filePath);

	if (!ofs.is_open())
	{
		std::cerr 
			<< "Error::Fruit::saveToFile(const char*) const::"
			<< "cannot open: " 
			<< filePath 
			<< '\n';
		assert(false);
		exit(-13);
		return;
	}

	ofs << m_energy << '\n';
	ofs << m_shape.getPosition().x << ' ' << m_shape.getPosition().y << '\n';
	ofs << m_shape.getRadius() << '\n';
	ofs << static_cast<int>(m_shape.getFillColor().r) << ' ';
	ofs << static_cast<int>(m_shape.getFillColor().g) << ' ';
	ofs << static_cast<int>(m_shape.getFillColor().b) << ' ';
	ofs << static_cast<int>(m_shape.getFillColor().a);

	ofs.close();
}

void Fruit::loadFromFile(const char* filePath)
{
	std::ifstream ifs(filePath);

	if (!ifs.is_open())
	{
		std::cerr
			<< "Error::Fruit::saveToFile(const char*)::"
			<< "cannot open: "
			<< filePath
			<< '\n';
		assert(false);
		exit(-13);
		return;
	}

	sf::Vector2f position;
	float radius;
	unsigned red, green, blue, alfa;

	ifs >> m_energy;
	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> red >> green >> blue >> alfa;

	initShape(position, radius, sf::Color(red, green, blue, alfa));

	ifs.close();
}

void Fruit::render(sf::RenderTarget& target) const
{
	target.draw(m_shape);
}

bool Fruit::isCoveredByMouse(const sf::Vector2f& mousePosView) const
{
	float x = m_shape.getPosition().x - mousePosView.x;
	float y = m_shape.getPosition().y - mousePosView.y;

	float distance = sqrt(pow(x, 2.0f) + pow(y, 2.0f));

	return getRadius() >= distance;
}

std::string Fruit::toStr() const
{
	std::stringstream ss;
	
	ss << "energy: " << getEnergy() << '\n';
	ss << "position: " << getPosition().x << ' ' << getPosition().y;

	return ss.str();
}

// accessors:

const sf::Vector2f& Fruit::getPosition() const
{
	return m_shape.getPosition();
}

float Fruit::getRadius() const
{
	return m_shape.getRadius();
}

unsigned Fruit::getEnergy() const
{
	return m_energy;
}

const sf::Color& Fruit::getColor() const
{
	return m_shape.getFillColor();
}

// mutators:

void Fruit::setPosition(float x, float y)
{
	m_shape.setPosition(x, y);
}

void Fruit::setPosition(const sf::Vector2f& newPosition)
{
	m_shape.setPosition(newPosition);
}

void Fruit::setRandomPosition(
	const sf::Vector2f& worldSize, 
	float bordersThickness)
{
	std::pair<unsigned, unsigned> rangeX = {
		bordersThickness + m_shape.getRadius(),
		worldSize.x - bordersThickness - m_shape.getRadius()
	};
	std::pair<unsigned, unsigned> rangeY = {
		bordersThickness + m_shape.getRadius(),
		worldSize.y - bordersThickness - m_shape.getRadius()
	};

	float x = Blueberry::RandomEngine::getScalarInRange(
		rangeX.first, 
		rangeX.second
	);
	float y = Blueberry::RandomEngine::getScalarInRange(
		rangeY.first, 
		rangeY.second
	);

	m_shape.setPosition(x, y);
}

void Fruit::setRadius(float radius)
{
	m_shape.setRadius(radius);
}

void Fruit::setEnergy(unsigned energy)
{
	m_energy = energy;
}

void Fruit::setColor(const sf::Color& color)
{
	m_shape.setFillColor(color);
}

// private methods:

void Fruit::initShape(
	const sf::Vector2f& position, 
	float radius, 
	const sf::Color& color)
{
	m_shape.setPosition(position);
	m_shape.setRadius(radius);
	m_shape.setOrigin(radius, radius);
	m_shape.setFillColor(color);
}
