#include "Animal.h"

Animal::Animal(
	const sf::Vector2f& position,
	float radius,
	const sf::Color& bodyColor,
	const sf::Color& hpBarBackgroundColor,
	const sf::Color& hpBarProgressRectColor,
	int defaultHp,
	float basalMetabolicRatePerFrame)
	: m_body()
	, m_movementComponent(nullptr)
	, m_alive(true)
	, m_isClone(false)
	, m_parentAgeWhenItWasBorn({ "no parent", 0.0f })
	, m_hpBar(nullptr)
	, m_brainPreview(nullptr)
	, m_age(0.0f)
	, m_timeElapsedSinceLastCloning({ "there hasn't been any cloning yet", 0.0f })
	, m_timeElapsedSinceLastExternalHpChange({ "there hasn't been any external hp change yet", 0.0f })
	, m_timeSinceLastMutation({ "hasn't mutated yet", 0.0f })
	, m_basalMetabolicRatePerFrame(basalMetabolicRatePerFrame)
	, m_energyToExpelFromBMR(0U)
{
	initMovementComponent();
	initBody(position, radius, bodyColor);
	initHpBar(defaultHp, hpBarBackgroundColor, hpBarProgressRectColor);
	initBrainPreview();
}

Animal::Animal(const char* folderPath)
	: m_body()
	, m_movementComponent(nullptr)
	, m_alive(true)
	, m_isClone(false)
	, m_parentAgeWhenItWasBorn({ "no parent", 0.0f })
	, m_hpBar(nullptr)
	, m_brainPreview(nullptr)
	, m_age(0.0f)
	, m_timeElapsedSinceLastCloning({ "there hasn't been any cloning yet", 0.0f })
	, m_timeElapsedSinceLastExternalHpChange({ "there hasn't been any external hp change yet", 0.0f })
	, m_timeSinceLastMutation({ "hasn't mutated yet", 0.0f })
	, m_basalMetabolicRatePerFrame(0U)
	, m_energyToExpelFromBMR(0U)
{
	this->loadFromFolder(folderPath);
}

Animal::Animal(const Animal& rhs)
	: m_body(rhs.m_body)
	, m_movementComponent(std::make_unique<MovementComponent>(*rhs.m_movementComponent))
	, m_alive(rhs.m_alive)
	, m_isClone(true)
	, m_parentAgeWhenItWasBorn({ "", rhs.m_age })
	, m_hpBar(std::make_unique<gui::IntProgressBar>(*rhs.m_hpBar))
	, m_brainPreview(nullptr)
	, m_age(0.0f)
	, m_timeElapsedSinceLastCloning({ "there hasn't been any cloning yet", 0.0f })
	, m_timeElapsedSinceLastExternalHpChange({ "there hasn't been any external hp change yet", 0.0f })
	, m_timeSinceLastMutation({ "hasn't mutated yet", 0.0f })
	, m_basalMetabolicRatePerFrame(rhs.m_basalMetabolicRatePerFrame)
	, m_energyToExpelFromBMR(rhs.m_energyToExpelFromBMR)
{
	initBrainPreview();
}

Animal& Animal::operator=(const Animal& rhs)
{
	if (this != &rhs)
	{
		m_body = rhs.m_body;
		*m_movementComponent = *rhs.m_movementComponent;
		m_alive = rhs.m_alive;
		m_isClone = true;
		m_parentAgeWhenItWasBorn = { "", rhs.m_age };
		*m_hpBar = *rhs.m_hpBar;

		initBrainPreview();

		m_age = 0.0f;
		m_timeElapsedSinceLastCloning = { "there hasn't been any cloning yet", 0.0f };
		m_timeElapsedSinceLastExternalHpChange = { "there hasn't been any external hp change yet", 0.0f };
		m_timeSinceLastMutation = { "hasn't mutated yet", 0.0f };
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
	ofs << m_isClone << '\n';
	ofs << m_parentAgeWhenItWasBorn.info << '\n';
	ofs << m_parentAgeWhenItWasBorn.number << '\n';
	ofs << m_hpBar->getCurrentValue() << '\n';
	ofs << m_movementComponent->getPreviousVelocityVector().x << '\n';
	ofs << m_movementComponent->getPreviousVelocityVector().y << '\n';
	ofs << m_movementComponent->getVelocityVector().x << '\n';
	ofs << m_movementComponent->getVelocityVector().y << '\n';
	ofs << m_movementComponent->getAccelerationVector().x << '\n';
	ofs << m_movementComponent->getAccelerationVector().y << '\n';
	ofs << m_movementComponent->getVelocityCoefficient() << '\n';

	ofs << m_age << '\n';
	
	ofs << m_timeElapsedSinceLastCloning.info << '\n';
	ofs << m_timeElapsedSinceLastCloning.number << '\n';
	
	ofs << m_timeElapsedSinceLastExternalHpChange.info << '\n';
	ofs << m_timeElapsedSinceLastExternalHpChange.number << '\n';
	
	ofs << m_timeSinceLastMutation.info << '\n';
	ofs << m_timeSinceLastMutation.number << '\n';
	
	ofs << m_basalMetabolicRatePerFrame << '\n';
	ofs << m_energyToExpelFromBMR;

	ofs.close();
}

void Animal::loadFromFolder(const char* folderPath)
{
	std::string brainFilePath = folderPath;
	brainFilePath += "/brain.ini";
	
	m_movementComponent = std::make_unique<MovementComponent>(brainFilePath.c_str());

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
	std::string temp;
	int hp;
	sf::Vector2i prevVelocity;
	sf::Vector2i velocity;
	sf::Vector2f acceleration;
	float velocityCoeffcient;

	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> bodyColorR >> bodyColorG >> bodyColorB >> bodyColorA;
	ifs >> hpBarBgColorR >> hpBarBgColorG >> hpBarBgColorB >> hpBarBgColorA;
	ifs >> hpBarColorR >> hpBarColorG >> hpBarColorB >> hpBarColorA;
	ifs >> m_alive;
	ifs >> m_isClone;
	
	std::getline(ifs, m_parentAgeWhenItWasBorn.info);
	std::getline(ifs, temp);
	
	ifs >> m_parentAgeWhenItWasBorn.number;
	ifs >> hp;

	ifs >> prevVelocity.x >> prevVelocity.y;
	ifs >> velocity.x >> velocity.y;
	ifs >> acceleration.x >> acceleration.y;
	ifs >> velocityCoeffcient;
	
	ifs >> m_age;

	std::getline(ifs, m_timeElapsedSinceLastCloning.info);
	std::getline(ifs, temp);
	ifs >> m_timeElapsedSinceLastCloning.number;
	
	std::getline(ifs, m_timeElapsedSinceLastExternalHpChange.info);
	std::getline(ifs, temp);
	ifs >> m_timeElapsedSinceLastExternalHpChange.number;
	
	std::getline(ifs, m_timeSinceLastMutation.info);
	std::getline(ifs, temp);
	ifs >> m_timeSinceLastMutation.number;

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

	m_movementComponent->setVelocitiesVectorsLoadedFromFile(velocity, prevVelocity);
	m_movementComponent->setAccelerationVectorLoadedFromFile(acceleration);
	m_movementComponent->setVelocityCoefficient(velocityCoeffcient);
}

void Animal::update(
	float dt,
	float simulationSpeedFactor,
	const std::vector<Blueberry::Scalar>& externalInputsForBrain,
	bool isTracked,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events,
	std::ofstream& debugFile,
	const std::unordered_map<std::string, int>& keybinds)
{
	if (m_timeSinceLastMutation.number > 30.0f) // TODO: unhardcode and change from seconds to frames
	{
		m_movementComponent->mutateBrain(1U);
		
		m_timeSinceLastMutation.info = "";
		m_timeSinceLastMutation.number = 0.0f;
	}
	
	doBMRrelatedThings();
	
	m_movementComponent->update(
		dt,
		m_hpBar->getCurrentValue(),
		simulationSpeedFactor,
		getEnhancedBrainInputs(externalInputsForBrain),
		isTracked,
		debugFile,
		keybinds
	);

	updateBody();
	updateHp();

	m_alive = m_hpBar->getCurrentValue() > 0;

	updateHpBarPosition();
	updateBrainPreview(mousePos, events);

	m_age += dt;
	m_timeElapsedSinceLastCloning.number += dt;
	m_timeElapsedSinceLastExternalHpChange.number += dt;	
	m_timeSinceLastMutation.number += dt;

	if (dt > 1.0)
	{
		std::cerr 
			<< "ERROR::Animal::update(...):\n"
			<< "dt is weirdly big : " << dt << '\n';
	}
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

	ss << "velocity coefficient: "
	   << m_movementComponent->getVelocityCoefficient()
	   << '\n';

	ss << "kinetic energy: " << getKineticEnergy() << '\n';
	
	ss << "alive (0 - no, 1 - yes): " << m_alive << '\n';
	ss << "clone (0 - no, 1 - yes): " << m_isClone << '\n';
	
	ss << "parent age when it was born: ";
	if (m_parentAgeWhenItWasBorn.info == "")
	{
		ss << m_parentAgeWhenItWasBorn.number << '\n';
	}
	else
	{
		ss << m_parentAgeWhenItWasBorn.info << '\n';
	}

	ss << "HP: " << getHp() << '\n';
	
	ss << "total energy: " << getTotalEnergy() << '\n';
	
	ss << "age: " << m_age << '\n';

	ss << "time since last mutation: ";
	if (m_timeSinceLastMutation.info == "")
	{
		ss << m_timeSinceLastMutation.number << '\n';
	}
	else
	{
		ss << m_timeSinceLastMutation.info << '\n';
	}
	
	ss << "time elapsed since last meal: ";
	if (m_timeElapsedSinceLastExternalHpChange.info == "")
	{
		ss << m_timeElapsedSinceLastExternalHpChange.number << '\n';
	}
	else
	{
		ss << m_timeElapsedSinceLastExternalHpChange.info << '\n';
	}

	ss << "time elapsed since last cloning: ";
	if (m_timeElapsedSinceLastCloning.info == "")
	{
		ss << m_timeElapsedSinceLastCloning.number << '\n';
	}
	else
	{
		ss << m_timeElapsedSinceLastCloning.info << '\n';
	}

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

bool Animal::isClone() const
{
	return m_isClone;
}

int Animal::getHp() const
{
	return m_hpBar->getCurrentValue();
}

const numberWithInfo<float>& Animal::getParentAgeWhenItWasBorn() const
{
	return m_parentAgeWhenItWasBorn;
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

const numberWithInfo<float>& Animal::getTimeElapsedSinceLastCloning() const
{
	return m_timeElapsedSinceLastCloning;
}

const numberWithInfo<float>& Animal::getTimeElapsedSinceLastExternalHpChange() const
{
	return m_timeElapsedSinceLastExternalHpChange;
}

const numberWithInfo<float>& Animal::getTimeElapsedSinceLastMutation() const
{
	return m_timeSinceLastMutation;
}

float Animal::getBasalMetabolicRatePerFrame() const
{
	return m_basalMetabolicRatePerFrame;
}

bool Animal::isCoveredByMouse(const sf::Vector2f& mousePosView) const
{
	float x = m_body.getPosition().x - mousePosView.x;
	float y = m_body.getPosition().y - mousePosView.y;

	return sqrt(pow(x, 2.0f) + pow(y, 2.0f)) <= m_body.getRadius();
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
	const std::vector<sf::Event>& events)
{
	m_movementComponent->mutateBrain(brainMutationsCount);
	m_brainPreview->update(mousePos, events);

	m_timeSinceLastMutation.info = "";
	m_timeSinceLastMutation.number = 0.0f;
}

void Animal::setAlive(bool alive)
{
	m_alive = alive;
}

void Animal::setHp(int hp)
{
	m_hpBar->setValue(hp);

	m_alive = m_hpBar->getCurrentValue() > 0;

	m_timeElapsedSinceLastExternalHpChange.info = "";
	m_timeElapsedSinceLastExternalHpChange.number = 0.0f;
}

void Animal::increaseHp(int hpIncrease)
{
	m_hpBar->increaseValue(hpIncrease);

	m_alive = m_hpBar->getCurrentValue() > 0.0;

	m_timeElapsedSinceLastExternalHpChange.info = "";
	m_timeElapsedSinceLastExternalHpChange.number = 0.0f;
}

void Animal::decreaseHp(int hpDecrease)
{
	m_hpBar->decreaseValue(hpDecrease);

	m_alive = m_hpBar->getCurrentValue() > 0.0;

	m_timeElapsedSinceLastExternalHpChange.info = "";
	m_timeElapsedSinceLastExternalHpChange.number = 0.0f;
}

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

void Animal::setBasalMetabolicRatePerFrame(float basalMetabolicRatePerFrame)
{
	m_basalMetabolicRatePerFrame = basalMetabolicRatePerFrame;
}

// private methods:

void Animal::resetTimeElapsedSinceLastCloning()
{
	m_timeElapsedSinceLastCloning.info = "";
	m_timeElapsedSinceLastCloning.number = 0.0f;
}

void Animal::initMovementComponent()
{
	m_movementComponent = std::make_unique<MovementComponent>(
		sf::Vector2i(0, 0),
		7U,
		3U,
		// TODO: unhardcode (or at least move it closer to inputs/connect it with them in some way)
		std::vector<std::string>{
			"input related to: direction to the nearest food (x)",
			"input related to: direction to the nearest food (y)",
			"input related to: position (x)",
			"input related to: position (y)",
			"input related to: velocity (x)",
			"input related to: velocity (y)",
			"input related to: hp",
			"output related to: acceleration (x)",
			"output related to: acceleration (y)",
			"output related to: cloning"
		}
	);
}

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
		sf::Color(128, 128, 128, 128)
	);
}

void Animal::updateBody()
{
	const sf::Vector2f& velVect = static_cast<sf::Vector2f>(m_movementComponent->getVelocityVector());

	m_body.setPosition(
		m_body.getPosition().x + velVect.x / m_movementComponent->getVelocityCoefficient(),
		m_body.getPosition().y + velVect.y / m_movementComponent->getVelocityCoefficient()
	);
}

void Animal::doBMRrelatedThings()
{
	float hp = static_cast<float>(m_hpBar->getCurrentValue());
	unsigned BMR = static_cast<unsigned>(m_basalMetabolicRatePerFrame * hp);

	BMR = std::max(BMR, 1U); // we don't want BMR to be equal to 0
	
	// BMR cannot decrease hp so that it's < 0:
	m_energyToExpelFromBMR = std::min(
		m_hpBar->getCurrentValue(),
		static_cast<int>(BMR)
	);

	// again: BMR cannot decrease hp so that it's < 0:
	m_hpBar->setValue(
		std::max(
			0,
			m_hpBar->getCurrentValue() - static_cast<int>(BMR)
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
	sf::Vector2i velocity = m_movementComponent->getVelocityVector();

	Blueberry::Scalar xVelocityInput = log10(abs(velocity.x) + 1);
	Blueberry::Scalar yVelocityInput = log10(abs(velocity.y) + 1);

	if (velocity.x > 0)
	{
		enhancedBrainInputs.emplace_back(xVelocityInput);
	}
	else
	{
		enhancedBrainInputs.emplace_back(-xVelocityInput);
	}

	if (velocity.y > 0)
	{
		enhancedBrainInputs.emplace_back(yVelocityInput);
	}
	else
	{
		enhancedBrainInputs.emplace_back(-yVelocityInput);
	}
	
	// log(0)=-inf, but log(1)=0 :)
	// but changing (for example) 10'000 to 10'001 is negligibly small :)
	enhancedBrainInputs.emplace_back(log10(m_hpBar->getCurrentValue() + 1));

	return enhancedBrainInputs;
}

void Animal::updateHp()
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
