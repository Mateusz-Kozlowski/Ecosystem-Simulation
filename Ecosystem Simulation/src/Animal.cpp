#include "stdafx.h"
#include "Animal.h"

Animal::Animal(
	const sf::Vector2f& position,
	float radius,
	const sf::Color& body_color,
	const sf::Color& hp_bar_background_color,
	const sf::Color& hp_bar_progress_rect_color,
	float default_hp,
	float max_hp)
	: maxHp(max_hp),
	  kineticEnergyFromPreviousFrame(0.0f),
	  alive(true),
	  movementComponent(std::make_unique<MovementComponent>())
{
	this->initBody(position, radius, body_color);	
	this->initHpBar(default_hp, max_hp, hp_bar_background_color, hp_bar_progress_rect_color);
	this->initBrainPreview();
}

Animal::Animal(const std::string& folder_path)
	: maxHp(0.0f),
	  kineticEnergyFromPreviousFrame(0.0f),
	  alive(true),
	  movementComponent(std::make_unique<MovementComponent>())
{
	this->loadFromFolder(folder_path);
}

Animal::Animal(const Animal& rhs)
	: movementComponent(std::make_unique<MovementComponent>()),
	  hpBar(std::make_unique<gui::ProgressBar>())
{
	this->body = rhs.body;
	this->maxHp = rhs.maxHp;
	*this->movementComponent = *rhs.movementComponent;
	this->kineticEnergyFromPreviousFrame = rhs.kineticEnergyFromPreviousFrame;
	this->alive = rhs.alive;
	*this->hpBar = *rhs.hpBar;

	this->initBrainPreview();
}

Animal& Animal::operator=(const Animal& rhs)
{
	if (this != &rhs)
	{
		this->body = rhs.body;
		this->maxHp = rhs.maxHp;
		*this->movementComponent = *rhs.movementComponent;
		this->kineticEnergyFromPreviousFrame = rhs.kineticEnergyFromPreviousFrame;
		this->alive = rhs.alive;
		*this->hpBar = *rhs.hpBar;

		this->initBrainPreview();
	}

	return *this;
}

// public methods:
void Animal::saveToFolder(const std::string& folder_path) const
{
	std::filesystem::create_directories(folder_path);

	this->movementComponent->saveBrainToFile(folder_path + "/brain.ini");

	std::string path = folder_path + "/animal.ini";

	std::ofstream ofs(path);

	if (!ofs.is_open())
	{
		std::cerr << "ERROR::Animal::saveToFolder::CANNOT OPEN: " << path << '\n';
		exit(-1);
	}

	ofs << this->body.getPosition().x << ' ' << this->body.getPosition().y << '\n';
	ofs << this->body.getRadius() << '\n';
	
	ofs << static_cast<int>(this->body.getFillColor().r) << ' ';
	ofs << static_cast<int>(this->body.getFillColor().g) << ' ';
	ofs << static_cast<int>(this->body.getFillColor().b) << ' ';
	ofs << static_cast<int>(this->body.getFillColor().a) << '\n';

	ofs << static_cast<int>(this->hpBar->getBackgroundColor().r) << ' ';
	ofs << static_cast<int>(this->hpBar->getBackgroundColor().g) << ' ';
	ofs << static_cast<int>(this->hpBar->getBackgroundColor().b) << ' ';
	ofs << static_cast<int>(this->hpBar->getBackgroundColor().a) << '\n';
	
	ofs << static_cast<int>(this->hpBar->getProgressRectColor().r) << ' ';
	ofs << static_cast<int>(this->hpBar->getProgressRectColor().g) << ' ';
	ofs << static_cast<int>(this->hpBar->getProgressRectColor().b) << ' ';
	ofs << static_cast<int>(this->hpBar->getProgressRectColor().a) << '\n';
	
	ofs << this->maxHp << '\n';
	ofs << this->kineticEnergyFromPreviousFrame << '\n';
	ofs << this->alive << '\n';
	ofs << this->hpBar->getCurrentValue() << '\n';
	ofs << this->movementComponent->get_vx() << '\n' << this->movementComponent->get_vy();

	ofs.close();
}

void Animal::loadFromFolder(const std::string& folder_path)
{
	this->movementComponent->loadBrainFromFile(folder_path + "/brain.ini");

	std::string path = folder_path + "/animal.ini";

	std::ifstream ifs(path);

	if (!ifs.is_open())
	{
		std::cerr << "ERROR::Animal::loadFromFolder::CANNOT OPEN: " << path << '\n';
		exit(-1);
	}

	sf::Vector2f position;
	float radius;
	unsigned bodyColorR, bodyColorG, bodyColorB, bodyColorA;
	unsigned hpBarBgColorR, hpBarBgColorG, hpBarBgColorB, hpBarBgColorA;
	unsigned hpBarProgressColorR, hpBarProgressColorG, hpBarProgressColorB, hpBarProgressColorA;
	float hp;
	sf::Vector2f velocity;

	ifs >> position.x >> position.y;
	ifs >> radius;
	ifs >> bodyColorR >> bodyColorG >> bodyColorB >> bodyColorA;
	ifs >> hpBarBgColorR >> hpBarBgColorG >> hpBarBgColorB >> hpBarBgColorA;
	ifs >> hpBarProgressColorR >> hpBarProgressColorG >> hpBarProgressColorB >> hpBarProgressColorA;
	ifs >> this->maxHp;
	ifs >> this->kineticEnergyFromPreviousFrame;
	ifs >> this->alive;
	ifs >> hp;
	ifs >> velocity.x >> velocity.y;

	ifs.close();

	this->initBody(position, radius, sf::Color(bodyColorR, bodyColorG, bodyColorB, bodyColorA));
	this->initHpBar(
		hp, 
		this->maxHp, 
		sf::Color(
			hpBarBgColorR,
			hpBarBgColorG,
			hpBarBgColorB,
			hpBarBgColorA
		), 
		sf::Color(
			hpBarProgressColorR,
			hpBarProgressColorG,
			hpBarProgressColorB,
			hpBarProgressColorA
		)
	);
	this->initBrainPreview();

	this->movementComponent->setVelocity(velocity);
}

void Animal::update(float dt, float simulation_speed_factor, const std::vector<double>& brain_inputs)
{
	this->kineticEnergyFromPreviousFrame = this->getKineticEnergy();

	this->movementComponent->update(dt, simulation_speed_factor, brain_inputs);

	this->updateBody(dt);
	this->updateHp(dt);

	this->alive = this->hpBar->getCurrentValue() > 0.0f;

	this->updateHpBarPosition();
	this->updateBrainPreview();
}

void Animal::renderBody(sf::RenderTarget& target) const
{
	target.draw(this->body);
}

void Animal::renderHpBar(sf::RenderTarget& target) const
{
	this->hpBar->render(target);
}

void Animal::renderBrainPreview(sf::RenderTarget& target) const
{
	this->brainPreview->render(target);
}

// accessors:
const sf::Vector2f& Animal::getPosition() const
{
	return this->body.getPosition();
}

float Animal::getRadius() const
{
	return this->body.getRadius();
}

const sf::Color& Animal::getColor() const
{
	return this->body.getFillColor();
}

float Animal::getMaxHp() const
{
	return this->maxHp;
}

const MovementComponent& Animal::getMovementComponent() const
{
	return *this->movementComponent;
}

const CrappyNeuralNets::TempNet& Animal::getBrain() const
{
	return this->movementComponent->getBrain();
}

const sf::Vector2f& Animal::getVelocityVector() const
{
	return this->movementComponent->getVelocityVector();
}

const sf::Vector2f& Animal::getAccelerationVector() const
{
	return this->movementComponent->getAccelerationVector();
}

float Animal::getVelocityVectorValue() const
{
	return this->movementComponent->getVelocityVectorValue();
}

float Animal::getAccelerationVectorValue() const
{
	return this->movementComponent->getAccelerationVectorValue();
}

float Animal::getKineticEnergy() const
{
	return 0.5f * pow(this->getVelocityVectorValue(), 2);
}

float Animal::getKineticEnergyDelta() const
{
	return this->getKineticEnergy() - this->kineticEnergyFromPreviousFrame;
}

float Animal::getEnergyToExpel() const
{
	if (this->getKineticEnergyDelta() >= 0.0f)
		return 0.0f;

	return -2.0f * this->getKineticEnergyDelta();
}

bool Animal::isAlive() const
{
	return this->alive;
}

float Animal::getHp() const
{
	return this->hpBar->getCurrentValue();
}

float Animal::getTotalEnergy() const
{
	return this->hpBar->getCurrentValue() + this->getKineticEnergy();
}

const gui::NeuralNetPreview& Animal::getBrainPreview() const
{
	return *this->brainPreview;
}

bool Animal::isCoveredByMouse(const sf::Vector2f& mouse_pos_view) const
{
	float x = this->body.getPosition().x - mouse_pos_view.x;
	float y = this->body.getPosition().y - mouse_pos_view.y;

	return sqrt(pow(x, 2) + pow(y, 2)) <= this->body.getRadius();
}

// mutators:
void Animal::setPosition(const sf::Vector2f& position)
{
	this->body.setPosition(position);

	this->updateHpBarPosition();
	this->brainPreview->setPosition(this->body.getPosition());
}

void Animal::setRandomPosition(const sf::Vector2f& world_size, float borders_thickness)
{
	std::pair<unsigned, unsigned> rangeX = {
		borders_thickness + this->body.getRadius(),
		world_size.x - borders_thickness - this->body.getRadius()
	};

	std::pair<unsigned, unsigned> rangeY = {
		borders_thickness + this->body.getRadius(),
		world_size.y - borders_thickness - this->body.getRadius()
	};

	float x = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(rangeX);
	float y = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(rangeY);

	this->body.setPosition(x, y);

	this->updateHpBarPosition();
	this->brainPreview->setPosition(this->body.getPosition());
}

void Animal::setRadius(float radius)
{
	this->body.setRadius(radius);
}

void Animal::setColor(const sf::Color& color)
{
	this->body.setFillColor(color);

	this->hpBar->setProgressRectColor(color);
}

void Animal::setMaxHp(float max_hp)
{
	if (this->hpBar->getCurrentValue() > max_hp)
	{
		std::cerr << "ERROR::Animal::setMaxHp::CANNOT SET MAX HP SMALLER THAT THE CURRENT HP\n";
		exit(-1);
	}

	this->maxHp = max_hp;
}

void Animal::randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage)
{
	this->movementComponent->randomMutate(mutation_percentage);
}

void Animal::setVelocity(const sf::Vector2f& velocity)
{
	this->movementComponent->setVelocity(velocity);
}

void Animal::setAlive(bool alive)
{
	this->alive = alive;
}

void Animal::setHp(float hp)
{
	if (hp > this->maxHp)
	{
		std::cerr << "ERROR::Animal::setHp::CANNOT SET HP GREATER THAN THE MAX HP\n";
		exit(-1);
	}

	this->hpBar->setValue(hp);

	this->alive = this->hpBar->getCurrentValue() > 0.0f;
}

void Animal::increaseHp(float hp_increase)
{
	if (this->hpBar->getCurrentValue() + hp_increase > this->maxHp)
	{
		std::cerr << "ERROR::Animal::increaseHp::CANNOT SET HP GREATER THAN THE MAX HP\n";
		exit(-1);
	}

	this->hpBar->increaseValue(hp_increase);

	this->alive = this->hpBar->getCurrentValue() > 0.0f;
}

void Animal::decreaseHp(float hp_decrease)
{
	if (this->hpBar->getCurrentValue() - hp_decrease > this->maxHp)
	{
		std::cerr << "ERROR::Animal::decreaseHp::CANNOT SET HP GREATER THAN THE MAX HP\n";
		exit(-1);
	}

	this->hpBar->decreaseValue(hp_decrease);

	this->alive = this->hpBar->getCurrentValue() > 0.0f;
}

void Animal::setBrainPreviewPosition(const sf::Vector2f& position)
{
	this->brainPreview->setPosition(position);
}

void Animal::setBrainPreviewPosition(float x, float y)
{
	this->brainPreview->setPosition(x, y);
}

// private methods:

// initialization:
void Animal::initBody(
	const sf::Vector2f& position,
	float radius,
	const sf::Color& color)
{
	this->body.setPosition(position);
	this->body.setRadius(radius);
	this->body.setOrigin(radius, radius);
	this->body.setFillColor(color);
}

void Animal::initHpBar(
	float default_hp,
	float max_hp,
	const sf::Color& hp_bar_background_color,
	const sf::Color& hp_bar_progress_rect_color)
{
	this->hpBar = std::make_unique<gui::ProgressBar>(
		sf::Vector2f(
			0.0f,
			max_hp
		),
		false,
		default_hp,
		sf::Vector2f(
			this->body.getPosition().x - 3.f * this->body.getRadius(),
			this->body.getPosition().y - 3.f * this->body.getRadius()
		),
		sf::Vector2f(
			6.f * this->body.getRadius(),
			this->body.getRadius()
		),
		hp_bar_background_color,
		hp_bar_progress_rect_color
	);
}

void Animal::initBrainPreview()
{
	// TODO: do sth with these hard-coded arguments:
	this->brainPreview = std::make_unique<gui::NeuralNetPreview>(
		this->movementComponent->getBrain(),
		this->body.getPosition(),
		sf::Vector2f(
			16.0f * this->body.getRadius(),
			16.0f * this->body.getRadius()
		),
		sf::Color(100, 100, 100)
	);
}

// private utilities:
void Animal::updateBody(float dt)
{
	this->body.setPosition(
		this->body.getPosition().x + this->movementComponent->get_vx() * dt,
		this->body.getPosition().y + this->movementComponent->get_vy() * dt
	);
}

void Animal::updateHp(float dt)
{
	this->hpBar->decreaseValue(abs(this->getKineticEnergyDelta()));
}

void Animal::updateHpBarPosition()
{
	this->hpBar->setPosition(
		sf::Vector2f(
			this->body.getPosition().x - 3.f * this->body.getRadius(),
			this->body.getPosition().y - 3.f * this->body.getRadius()
		)
	);
}

void Animal::updateBrainPreview()
{
	this->brainPreview->setPosition(this->body.getPosition());
	this->brainPreview->update();
}
