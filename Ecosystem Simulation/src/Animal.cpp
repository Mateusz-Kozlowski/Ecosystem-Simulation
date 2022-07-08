#include "Animal.h"

Animal::Animal(
	const sf::Vector2f& position,
	float radius,
	const sf::Color& bodyColor,
	const sf::Color& hpBarBackgroundColor,
	const sf::Color& hpBarProgressRectColor,
	int defaultHp,
	unsigned basalMetabolicRatePerFrame)
	: m_body()
	, m_movementComponent(std::make_unique<MovementComponent>())
	, m_alive(true)
	, m_hpBar(nullptr)
	, m_brainPreview(nullptr)
	, m_timeElapsedSinceLastExternalHpChange(0.0f)
	, m_age(0.0f)
	, m_basalMetabolicRatePerFrame(basalMetabolicRatePerFrame)
	, m_energyToExpelFromBMR(0U)
{
	this->initBody(position, radius, bodyColor);
	this->initHpBar(defaultHp, hpBarBackgroundColor, hpBarProgressRectColor);
	this->initBrainPreview();
}

Animal::Animal(const char* folderPath)
	: m_body()
	, m_movementComponent(std::make_unique<MovementComponent>())
	, m_alive(true)
	, m_hpBar(nullptr)
	, m_brainPreview(nullptr)
	, m_timeElapsedSinceLastExternalHpChange(0.0f)
	, m_age(0.0f)
	, m_basalMetabolicRatePerFrame(0U)
	, m_energyToExpelFromBMR(0U)
{
	this->loadFromFolder(folderPath);
}

Animal::Animal(const Animal& rhs)
	: m_body(rhs.m_body)
	, m_movementComponent(std::make_unique<MovementComponent>())
	, m_alive(rhs.m_alive)
	, m_hpBar(std::make_unique<gui::IntProgressBar>())
	, m_brainPreview(nullptr)
	, m_timeElapsedSinceLastExternalHpChange(rhs.m_timeElapsedSinceLastExternalHpChange)
	, m_age(0.0f)
	, m_basalMetabolicRatePerFrame(rhs.m_basalMetabolicRatePerFrame)
	, m_energyToExpelFromBMR(rhs.m_energyToExpelFromBMR)
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
		*m_movementComponent = *rhs.m_movementComponent;
		m_alive = rhs.m_alive;
		*m_hpBar = *rhs.m_hpBar;

		initBrainPreview();

		m_timeElapsedSinceLastExternalHpChange = rhs.m_timeElapsedSinceLastExternalHpChange;
		m_age = 0.0f;
		m_basalMetabolicRatePerFrame = rhs.m_basalMetabolicRatePerFrame;
		m_energyToExpelFromBMR = rhs.m_energyToExpelFromBMR;
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

	ofs << m_alive << '\n';
	ofs << m_hpBar->getCurrentValue() << '\n';
	ofs << m_movementComponent->getPreviousVelocityVector().x << '\n';
	ofs << m_movementComponent->getPreviousVelocityVector().y << '\n';
	ofs << m_movementComponent->getVelocityVector().x << '\n';
	ofs << m_movementComponent->getVelocityVector().y << '\n';
	ofs << m_timeElapsedSinceLastExternalHpChange << '\n';
	ofs << m_age << '\n';
	ofs << m_basalMetabolicRatePerFrame << '\n';
	ofs << m_energyToExpelFromBMR;

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
	int hp;
	sf::Vector2i prevVelocity;
	sf::Vector2i velocity;

	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> bodyColorR >> bodyColorG >> bodyColorB >> bodyColorA;
	ifs >> hpBarBgColorR >> hpBarBgColorG >> hpBarBgColorB >> hpBarBgColorA;
	ifs >> hpBarColorR >> hpBarColorG >> hpBarColorB >> hpBarColorA;
	ifs >> m_alive;
	ifs >> hp;
	ifs >> prevVelocity.x >> prevVelocity.y;
	ifs >> velocity.x >> velocity.y;
	ifs >> m_timeElapsedSinceLastExternalHpChange;
	ifs >> m_age;
	ifs >> m_basalMetabolicRatePerFrame;
	ifs >> m_energyToExpelFromBMR;

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

	m_movementComponent->setVelocitiesLoadedFromFile(velocity, prevVelocity);
}

void Animal::update(
	float dt,
	float simulationSpeedFactor,
	const std::vector<Blueberry::Scalar>& externalInputsForBrain,
	bool isTracked,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events,
	std::ofstream& debugFile)
{
	doBMRrelatedThings();
	
	m_movementComponent->update(
		dt,
		m_hpBar->getCurrentValue(),
		simulationSpeedFactor,
		getEnhancedBrainInputs(externalInputsForBrain),
		isTracked,
		debugFile
	);

	updateBody(dt);
	updateHp(dt);

	m_alive = m_hpBar->getCurrentValue() > 0;

	updateHpBarPosition();
	updateBrainPreview(mousePos, events);

	m_timeElapsedSinceLastExternalHpChange += dt;
	m_age += dt;
}

void Animal::updateOnlyImgBtnOfBrainPreview(
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	m_brainPreview->updateOnlyImgBtn(mousePos, events);
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
	m_brainPreview->render(target, true);
}

std::string Animal::toStr() const
{
	std::stringstream ss;

	ss << "position: "
	   << getPos().x << ' ' << getPos().y
	   << '\n';

	ss << "velocity: "
	   << getVelocityVector().x << ' ' << getVelocityVector().y
	   << '\n';

	ss << "acceleration: "
	   << getAccelerationVector().x << ' ' << getAccelerationVector().y
	   << '\n';

	ss << "kinetic energy: " << getKineticEnergy() << '\n';
	
	ss << "HP: " << getHp() << '\n';
	
	ss << "total energy: " << getTotalEnergy() << '\n';
	
	ss << "age: " << m_age << '\n';

	ss << "time elapsed since last meal: "
	   << m_timeElapsedSinceLastExternalHpChange << '\n';

	ss << "Basal Metabolic Rate per frame: "
	   << m_basalMetabolicRatePerFrame;

	return ss.str();
}

// accessors:

const sf::Vector2f& Animal::getPos() const
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

MovementComponent& Animal::getMovementComponent() const
{
	return *m_movementComponent;
}

const Blueberry::Brain& Animal::getBrain() const
{
	return m_movementComponent->getBrain();
}

unsigned Animal::getEnergyToExpel() const
{
	return m_movementComponent->getEnergyToExpel() + m_energyToExpelFromBMR;
}

int Animal::getKineticEnergyDelta() const
{
	return m_movementComponent->getKineticEnergyDelta();
}

unsigned Animal::getPreviousKineticEnergy() const
{
	return m_movementComponent->getPreviousKineticEnergy();
}

unsigned Animal::getKineticEnergy() const
{
	return m_movementComponent->getKineticEnergy();
}

unsigned Animal::getPreviousVelocityVectorSquaredValue() const
{
	return m_movementComponent->getPreviousVelocityVectorSquaredValue();
}

unsigned Animal::getVelocityVectorSquaredValue() const
{
	return m_movementComponent->getVelocityVectorSquaredValue();
}

float Animal::getAccelerationVectorValue() const
{
	return m_movementComponent->getAccelerationVectorValue();
}

const sf::Vector2i& Animal::getPreviousVelocityVector() const
{
	return m_movementComponent->getPreviousVelocityVector();
}

const sf::Vector2i& Animal::getVelocityVector() const
{
	return m_movementComponent->getVelocityVector();
}

const sf::Vector2f& Animal::getAccelerationVector() const
{
	return m_movementComponent->getAccelerationVector();
}

bool Animal::isAlive() const
{
	return m_alive;
}

int Animal::getHp() const
{
	return m_hpBar->getCurrentValue();
}

unsigned Animal::getTotalEnergy() const
{
	return m_hpBar->getCurrentValue() + getKineticEnergy();
}

const std::unique_ptr<gui::IntProgressBar>& Animal::getHpBar() const
{
	return m_hpBar;
}

gui::BrainPreview& Animal::getBrainPreview() const
{
	return *m_brainPreview;
}

float Animal::getAge() const
{
	return m_age;
}

float Animal::getTimeElapsedSinceLastExternalHpChange() const
{
	return m_timeElapsedSinceLastExternalHpChange;
}

bool Animal::isCoveredByMouse(const sf::Vector2f& mousePosView) const
{
	float x = m_body.getPosition().x - mousePosView.x;
	float y = m_body.getPosition().y - mousePosView.y;

	return sqrt(pow(x, 2.0f) + pow(y, 2.0f)) <= m_body.getRadius();
}

unsigned Animal::getBasalMetabolicRatePerFrame() const
{
	return m_basalMetabolicRatePerFrame;
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

void Animal::randomMutate(
	unsigned brainMutationsCount,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events
)
{
	m_movementComponent->mutateBrain(brainMutationsCount);
	m_brainPreview->update(mousePos, events);
}

void Animal::setAlive(bool alive)
{
	m_alive = alive;
}

void Animal::setHp(int hp)
{
	m_hpBar->setValue(hp);

	m_alive = m_hpBar->getCurrentValue() > 0;

	m_timeElapsedSinceLastExternalHpChange = 0.0f;
}

//void Animal::increaseHp(double hpIncrease)
//{
//	m_hpBar->increaseValue(hpIncrease);
//
//	m_alive = m_hpBar->getCurrentValue() > 0.0;
//
//	m_timeElapsedSinceLastExternalHpChange = 0.0f;
//}

//void Animal::decreaseHp(double hpDecrease)
//{
//	m_hpBar->decreaseValue(hpDecrease);
//
//	m_alive = m_hpBar->getCurrentValue() > 0.0;
//
//	m_timeElapsedSinceLastExternalHpChange = 0.0f;
//}

void Animal::setHpBarRange(const sf::Vector2i& range)
{
	m_hpBar->setValuesRange(range);
}

void Animal::setBrainPreviewPosition(const sf::Vector2f& position)
{
	m_brainPreview->setPosition(position);
}

void Animal::setBrainPreviewPosition(float x, float y)
{
	m_brainPreview->setPosition(x, y);
}

void Animal::setBasalMetabolicRatePerFrame(unsigned basalMetabolicRatePerFrame)
{
	m_basalMetabolicRatePerFrame = basalMetabolicRatePerFrame;
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
	int defaultHp,
	const sf::Color& hpBarBackgroundColor,
	const sf::Color& hpBarProgressRectColor)
{
	m_hpBar = std::make_unique<gui::IntProgressBar>(
		sf::Vector2i(
			0,
			defaultHp
		),
		defaultHp,
		sf::Vector2f(
			m_body.getPosition().x - 3.0f * m_body.getRadius(),
			m_body.getPosition().y - 3.0f * m_body.getRadius()
		),
		sf::Vector2f(
			6.0f * m_body.getRadius(),
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
	const sf::Vector2f& velVect = static_cast<sf::Vector2f>(m_movementComponent->getVelocityVector());

	m_body.setPosition(
		m_body.getPosition().x + velVect.x * dt,
		m_body.getPosition().y + velVect.y * dt
	);
}

void Animal::doBMRrelatedThings()
{
	m_energyToExpelFromBMR = std::min(
		m_hpBar->getCurrentValue(),
		static_cast<int>(m_basalMetabolicRatePerFrame)
	);

	m_hpBar->setValue(
		std::max(
			0,
			m_hpBar->getCurrentValue() - static_cast<int>(m_basalMetabolicRatePerFrame)
		)
	);
}

std::vector<Blueberry::Scalar> Animal::getEnhancedBrainInputs(
	const std::vector<Blueberry::Scalar>& externalBrainInputs) const
{
	std::vector<Blueberry::Scalar> enhancedBrainInputs;
	
	enhancedBrainInputs.reserve(7);

	for (int i = 0; i < externalBrainInputs.size(); i++)
	{
		enhancedBrainInputs.emplace_back(externalBrainInputs[i]);
	}

	// now it's time to enhance the inputs:
	// log(0)=-inf, but log(1)=0 :)
	// but changing (for example) 10'000 to 10'001 is negligibly small :)
	enhancedBrainInputs.emplace_back(sqrt(abs(m_movementComponent->getVelocityVector().x)));
	enhancedBrainInputs.emplace_back(sqrt(abs(m_movementComponent->getVelocityVector().y)));

	// log(0)=-inf, but log(1)=0 :)
	// but changing (for example) 10'000 to 10'001 is negligibly small :)
	enhancedBrainInputs.emplace_back(log10(m_hpBar->getCurrentValue() + 1));

	return enhancedBrainInputs;
}

void Animal::updateHp(float dt)
{
	m_hpBar->decreaseValue(abs(getKineticEnergyDelta()));
}

void Animal::updateHpBarPosition()
{
	m_hpBar->setPosition(
		sf::Vector2f(
			m_body.getPosition().x - 3.0f * m_body.getRadius(),
			m_body.getPosition().y - 3.0f * m_body.getRadius()
		)
	);
}

void Animal::updateBrainPreview(
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	m_brainPreview->setPosition(m_body.getPosition());
	m_brainPreview->update(mousePos, events);
}
