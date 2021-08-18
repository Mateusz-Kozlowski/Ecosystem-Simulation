#include "pch.h"
#include "Animal.h"

Animal::Animal(
	const sf::Vector2f& position,
	float radius,
	const sf::Color& bodyColor,
	const sf::Color& hpBarBackgroundColor,
	const sf::Color& hpBarProgressRectColor,
	float defaultHp,
	float maxHp)
	: m_body()
	, m_maxHp(maxHp)
	, m_movementComponent(std::make_unique<MovementComponent>())
	, m_kineticEnergyFromPreviousFrame(0.0f)
	, m_alive(true)
	, m_hpBar(nullptr)
	, m_brainPreview(nullptr)
	, m_timeElapsedSinceLastExternalHpChange(0.0f)
{
	this->initBody(position, radius, bodyColor);
	this->initHpBar(defaultHp, hpBarBackgroundColor, hpBarProgressRectColor);
	this->initBrainPreview();
}

Animal::Animal(const char* folderPath)
	: m_body()
	, m_maxHp(0.0f)
	, m_movementComponent(std::make_unique<MovementComponent>())
	, m_kineticEnergyFromPreviousFrame(0.0f)
	, m_alive(true)
	, m_hpBar(nullptr)
	, m_brainPreview(nullptr)
	, m_timeElapsedSinceLastExternalHpChange(0.0f)
{
	this->loadFromFolder(folderPath);
}

Animal::Animal(const Animal& rhs)
	: m_body(rhs.m_body)
	, m_maxHp(rhs.m_maxHp)
	, m_movementComponent(std::make_unique<MovementComponent>())
	, m_kineticEnergyFromPreviousFrame(rhs.m_kineticEnergyFromPreviousFrame)
	, m_alive(rhs.m_alive)
	, m_hpBar(std::make_unique<gui::ProgressBar>())
	, m_brainPreview(nullptr)
	, m_timeElapsedSinceLastExternalHpChange(rhs.m_timeElapsedSinceLastExternalHpChange)
{
	*m_movementComponent = *rhs.m_movementComponent;
	*m_hpBar = *rhs.m_hpBar;

	initBrainPreview();
}

Animal& Animal::operator=(const Animal& rhs)
{
	if (this != &rhs)
	{
		m_body = rhs.m_body;
		m_maxHp = rhs.m_maxHp;
		*m_movementComponent = *rhs.m_movementComponent;
		m_kineticEnergyFromPreviousFrame = rhs.m_kineticEnergyFromPreviousFrame;
		m_alive = rhs.m_alive;
		*m_hpBar = *rhs.m_hpBar;

		initBrainPreview();

		m_timeElapsedSinceLastExternalHpChange = rhs.m_timeElapsedSinceLastExternalHpChange;
	}

	return *this;
}

void Animal::saveToFolder(const char* folderPath) const
{
	if (folderPath != "")
	{
		std::filesystem::create_directories(folderPath);
	}

	std::string brainFilePath;

	if (folderPath)
	{
		brainFilePath = folderPath;
	}
	
	brainFilePath += "/brain.ini";
	
	m_movementComponent->saveBrainToFile(brainFilePath.c_str());
	
	std::string animalFilePath;

	if (folderPath)
	{
		animalFilePath = folderPath;
	}

	animalFilePath += "/animal.ini";
	
	std::ofstream ofs(animalFilePath);

	if (!ofs.is_open())
	{
		std::cerr
			<< "Error::Animal::saveToFolder(const char*)::cannot open: "
			<< animalFilePath
			<< '\n';
		exit(-1);
	}

	ofs << m_body.getPosition().x << ' ' << m_body.getPosition().y << '\n';
	ofs << m_body.getRadius() << '\n';

	ofs << static_cast<int>(m_body.getFillColor().r) << ' ';
	ofs << static_cast<int>(m_body.getFillColor().g) << ' ';
	ofs << static_cast<int>(m_body.getFillColor().b) << ' ';
	ofs << static_cast<int>(m_body.getFillColor().a) << '\n';

	ofs << static_cast<int>(m_hpBar->getBackgroundColor().r) << ' ';
	ofs << static_cast<int>(m_hpBar->getBackgroundColor().g) << ' ';
	ofs << static_cast<int>(m_hpBar->getBackgroundColor().b) << ' ';
	ofs << static_cast<int>(m_hpBar->getBackgroundColor().a) << '\n';

	ofs << static_cast<int>(m_hpBar->getProgressRectColor().r) << ' ';
	ofs << static_cast<int>(m_hpBar->getProgressRectColor().g) << ' ';
	ofs << static_cast<int>(m_hpBar->getProgressRectColor().b) << ' ';
	ofs << static_cast<int>(m_hpBar->getProgressRectColor().a) << '\n';

	ofs << m_maxHp << '\n';
	ofs << m_kineticEnergyFromPreviousFrame << '\n';
	ofs << m_alive << '\n';
	ofs << m_hpBar->getCurrentValue() << '\n';
	ofs << m_movementComponent->getVelocityVector().x << '\n';
	ofs << m_movementComponent->getVelocityVector().y << '\n';
	ofs << m_timeElapsedSinceLastExternalHpChange;

	ofs.close();
}

void Animal::loadFromFolder(const char* folderPath)
{
	std::string brainFilePath = folderPath;
	brainFilePath += "/brain.ini";

	m_movementComponent->loadBrainFromFile(brainFilePath.c_str());

	std::string path = folderPath;
	path += "/animal.ini";

	std::ifstream ifs(path);

	if (!ifs.is_open())
	{
		std::cerr
			<< "Error::Animal::loadFromFolder(const char*)::cannot open: "
			<< path
			<< '\n';
		assert(false);
		return;
	}

	sf::Vector2f position;
	float radius;
	unsigned bodyColorR, bodyColorG, bodyColorB, bodyColorA;
	unsigned hpBarBgColorR, hpBarBgColorG, hpBarBgColorB, hpBarBgColorA;
	unsigned hpBarColorR, hpBarColorG, hpBarColorB, hpBarColorA;
	float hp;
	sf::Vector2f velocity;

	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> bodyColorR >> bodyColorG >> bodyColorB >> bodyColorA;
	ifs >> hpBarBgColorR >> hpBarBgColorG >> hpBarBgColorB >> hpBarBgColorA;
	ifs >> hpBarColorR >> hpBarColorG >> hpBarColorB >> hpBarColorA;
	ifs >> m_maxHp;
	ifs >> m_kineticEnergyFromPreviousFrame;
	ifs >> m_alive;
	ifs >> hp;
	ifs >> velocity.x >> velocity.y;
	ifs >> m_timeElapsedSinceLastExternalHpChange;

	ifs.close();

	initBody(
		position,
		radius,
		sf::Color(
			bodyColorR,
			bodyColorG,
			bodyColorB,
			bodyColorA
		)
	);

	initHpBar(
		hp,
		sf::Color(
			hpBarBgColorR,
			hpBarBgColorG,
			hpBarBgColorB,
			hpBarBgColorA
		),
		sf::Color(
			hpBarColorR,
			hpBarColorG,
			hpBarColorB,
			hpBarColorA
		)
	);

	initBrainPreview();

	m_movementComponent->setVelocity(velocity);
}

void Animal::update(
	float dt,
	float simulationSpeedFactor,
	const std::vector<double>& brainInputs)
{
	m_kineticEnergyFromPreviousFrame = getKineticEnergy();

	m_movementComponent->update(dt, simulationSpeedFactor, brainInputs);

	updateBody(dt);
	updateHp(dt);

	m_alive = m_hpBar->getCurrentValue() > 0.0f;

	updateHpBarPosition();
	updateBrainPreview();

	m_timeElapsedSinceLastExternalHpChange += dt;
}

void Animal::renderBody(sf::RenderTarget& target) const
{
	target.draw(m_body);
}

void Animal::renderHpBar(sf::RenderTarget& target) const
{
	m_hpBar->render(target);
}

void Animal::renderBrainPreview(sf::RenderTarget& target) const
{
	m_brainPreview->render(target);
}

// accessors:

const sf::Vector2f& Animal::getPosition() const
{
	return m_body.getPosition();
}

float Animal::getRadius() const
{
	return m_body.getRadius();
}

const sf::Color& Animal::getColor() const
{
	return m_body.getFillColor();
}

float Animal::getMaxHp() const
{
	return m_maxHp;
}

const MovementComponent& Animal::getMovementComponent() const
{
	return *m_movementComponent;
}

const Blueberry::Brain& Animal::getBrain() const
{
	return m_movementComponent->getBrain();
}

const sf::Vector2f& Animal::getVelocityVector() const
{
	return m_movementComponent->getVelocityVector();
}

const sf::Vector2f& Animal::getAccelerationVector() const
{
	return m_movementComponent->getAccelerationVector();
}

float Animal::getVelocityVectorValue() const
{
	return m_movementComponent->getVelocityVectorValue();
}

float Animal::getAccelerationVectorValue() const
{
	return m_movementComponent->getAccelerationVectorValue();
}

float Animal::getKineticEnergy() const
{
	return 0.5f * pow(getVelocityVectorValue(), 2);
}

float Animal::getKineticEnergyDelta() const
{
	return getKineticEnergy() - m_kineticEnergyFromPreviousFrame;
}

float Animal::getEnergyToExpel() const
{
	if (getKineticEnergyDelta() >= 0.0f) return 0.0f;

	return -2.0f * getKineticEnergyDelta();
}

bool Animal::isAlive() const
{
	return m_alive;
}

float Animal::getHp() const
{
	return m_hpBar->getCurrentValue();
}

float Animal::getTotalEnergy() const
{
	return m_hpBar->getCurrentValue() + getKineticEnergy();
}

const gui::BrainPreview& Animal::getBrainPreview() const
{
	return *m_brainPreview;
}

float Animal::getTimeElapsedSinceLastExternalHpChange() const
{
	return m_timeElapsedSinceLastExternalHpChange;
}

bool Animal::isCoveredByMouse(const sf::Vector2f& mousePosView) const
{
	float x = m_body.getPosition().x - mousePosView.x;
	float y = m_body.getPosition().y - mousePosView.y;

	return sqrt(pow(x, 2) + pow(y, 2)) <= m_body.getRadius();
}

// mutators:

void Animal::setPosition(const sf::Vector2f& position)
{
	m_body.setPosition(position);

	updateHpBarPosition();
	m_brainPreview->setPosition(m_body.getPosition());
}

void Animal::setRandomPosition(
	const sf::Vector2f& worldSize,
	float bordersThickness)
{
	std::pair<unsigned, unsigned> rangeX = {
		bordersThickness + m_body.getRadius(),
		worldSize.x - bordersThickness - m_body.getRadius()
	};
	std::pair<unsigned, unsigned> rangeY = {
		bordersThickness + m_body.getRadius(),
		worldSize.y - bordersThickness - m_body.getRadius()
	};

	float x = Blueberry::RandomEngine::getScalarInRange(
		rangeX.first,
		rangeX.second
	);
	float y = Blueberry::RandomEngine::getScalarInRange(
		rangeY.first,
		rangeY.second
	);

	m_body.setPosition(x, y);

	updateHpBarPosition();
	m_brainPreview->setPosition(m_body.getPosition());
}

void Animal::setRadius(float radius)
{
	m_body.setRadius(radius);
}

void Animal::setColor(const sf::Color& color)
{
	m_body.setFillColor(color);

	m_hpBar->setProgressRectColor(color);
}

void Animal::setMaxHp(float maxHp)
{
	if (m_hpBar->getCurrentValue() > maxHp)
	{
		std::cerr
			<< "Error::Animal::setMaxHp(float)::"
			<< "cannot set max hp smaller than the current hp\n";
		assert(false);
	}

	m_maxHp = maxHp;
}

void Animal::randomMutate(unsigned brainMutationsCount)
{
	m_movementComponent->mutateBrain(brainMutationsCount);
	m_brainPreview->update();
}

void Animal::setVelocity(const sf::Vector2f& velocity)
{
	m_movementComponent->setVelocity(velocity);
}

void Animal::setAlive(bool alive)
{
	m_alive = alive;
}

void Animal::setHp(float hp)
{
	if (hp > m_maxHp)
	{
		std::cerr
			<< "Error::Animal::setHp(float)::"
			<< "cannot set hp greater than the max hp\n";
		assert(false);
	}

	m_hpBar->setValue(hp);

	m_alive = m_hpBar->getCurrentValue() > 0.0f;

	m_timeElapsedSinceLastExternalHpChange = 0.0f;
}

void Animal::increaseHp(float hpIncrease)
{
	if (m_hpBar->getCurrentValue() + hpIncrease > m_maxHp)
	{
		std::cerr
			<< "Error::Animal::increaseHp(float)::"
			<< "cannot set hp greater than the max hp\n";
		assert(false);
	}

	m_hpBar->increaseValue(hpIncrease);

	m_alive = m_hpBar->getCurrentValue() > 0.0f;

	m_timeElapsedSinceLastExternalHpChange = 0.0f;
}

void Animal::decreaseHp(float hpDecrease)
{
	if (m_hpBar->getCurrentValue() - hpDecrease > m_maxHp)
	{
		std::cerr
			<< "Error::Animal::decreaseHp(float)::"
			<< "cannot set hp greater than the max hp\n";
		assert(false);
	}

	m_hpBar->decreaseValue(hpDecrease);

	m_alive = m_hpBar->getCurrentValue() > 0.0f;

	m_timeElapsedSinceLastExternalHpChange = 0.0f;
}

void Animal::setBrainPreviewPosition(const sf::Vector2f& position)
{
	m_brainPreview->setPosition(position);
}

void Animal::setBrainPreviewPosition(float x, float y)
{
	m_brainPreview->setPosition(x, y);
}

// private methods:

void Animal::initBody(
	const sf::Vector2f& position,
	float radius,
	const sf::Color& color)
{
	m_body.setPosition(position);
	m_body.setRadius(radius);
	m_body.setOrigin(radius, radius);
	m_body.setFillColor(color);
}

void Animal::initHpBar(
	float defaultHp,
	const sf::Color& hpBarBackgroundColor,
	const sf::Color& hpBarProgressRectColor)
{
	m_hpBar = std::make_unique<gui::ProgressBar>(
		sf::Vector2f(
			0.0f,
			m_maxHp
		),
		false,
		defaultHp,
		sf::Vector2f(
			m_body.getPosition().x - 3.f * m_body.getRadius(),
			m_body.getPosition().y - 3.f * m_body.getRadius()
		),
		sf::Vector2f(
			6.f * m_body.getRadius(),
			m_body.getRadius()
		),
		hpBarBackgroundColor,
		hpBarProgressRectColor
		);
}

void Animal::initBrainPreview()
{
	// TODO: do sth with these hard-coded arguments:
	m_brainPreview = std::make_unique<gui::BrainPreview>(
		m_movementComponent->getBrain(),
		m_body.getPosition(),
		sf::Vector2f(
			16.0f * m_body.getRadius(),
			16.0f * m_body.getRadius()
		),
		sf::Color(128, 128, 128)
	);
}

void Animal::updateBody(float dt)
{
	const sf::Vector2f& velVect = m_movementComponent->getVelocityVector();

	m_body.setPosition(
		m_body.getPosition().x + velVect.x * dt,
		m_body.getPosition().y + velVect.y * dt
	);
}

void Animal::updateHp(float dt)
{
	m_hpBar->decreaseValue(abs(getKineticEnergyDelta()));
}

void Animal::updateHpBarPosition()
{
	m_hpBar->setPosition(
		sf::Vector2f(
			m_body.getPosition().x - 3.f * m_body.getRadius(),
			m_body.getPosition().y - 3.f * m_body.getRadius()
		)
	);
}

void Animal::updateBrainPreview()
{
	m_brainPreview->setPosition(m_body.getPosition());
	m_brainPreview->update();
}
