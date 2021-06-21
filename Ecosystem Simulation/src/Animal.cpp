#include "stdafx.h"
#include "Animal.h"

// public static methods:
void Animal::setUpAnimalFolder(const std::string& folder_path)
{
	std::filesystem::create_directories(folder_path);

	MovementComponent movementComponent;

	movementComponent.set_x(1920.f);
	movementComponent.set_y(1080.f);

	movementComponent.saveToFolder(folder_path);
}

// constructor:
Animal::Animal(float max_hp, float default_hp, bool hp_bar_is_rendered, bool brain_is_rendered)
	: alive(true), 
	  maxHp(max_hp), hp(default_hp),
	  hpBarIsRendered(hp_bar_is_rendered), brainIsRendered(brain_is_rendered),
	  copied("default")
{
	this->initBody();
	this->initMovementComponent();
	this->initHpBar();
	this->initBrainPreview();
}

Animal::~Animal()
{
	delete this->movementComponent;
	delete this->hpBar;
	delete this->brainPreview;
}

void Animal::copyConstructor(const Animal& animal)
{
	this->alive = animal.alive;
	this->body = animal.body;
	this->movementComponent->copyConstructor(animal.getMovementComponent());	
	this->maxHp = animal.maxHp;
	this->hp = animal.hp;
	this->hpBarIsRendered = animal.hpBarIsRendered;
	this->brainIsRendered = animal.brainIsRendered;	
	*this->hpBar = *animal.hpBar;	
	this->brainPreview = new NeuralNetPreview(
		this->movementComponent->getBrain(),
		this->movementComponent->getPosition(),
		sf::Vector2f(144.f, 144.f),
		sf::Color(100, 100, 100)
	);

	// TODO: rmv later!:
	this->copied = "copied";
}

// initialization:
void Animal::loadFromFolder(const std::string& folder_path)
{	
	this->movementComponent->loadFromFolder(folder_path);

	this->body.setPosition(this->movementComponent->getPosition());

	this->hpBar->setPosition(
		sf::Vector2f(
			this->movementComponent->getPosition().x - 4.f * this->body.getRadius(),
			this->movementComponent->getPosition().y - 3.f * this->body.getRadius()
		)
	);

	this->brainPreview->update(this->movementComponent->getPosition());
}

// accessors:
const sf::Vector2f& Animal::getPosition() const
{
	return this->movementComponent->getPosition();
}

const sf::Vector2f& Animal::getVelocity() const
{
	return this->movementComponent->getVelocity();
}

float Animal::getRadius() const
{
	return this->body.getRadius();
}

bool Animal::isHpBarRendered() const
{
	return this->hpBarIsRendered;
}

bool Animal::isBrainRendered() const
{
	return this->brainIsRendered;
}

bool Animal::isAlive() const
{
	return this->alive;
}

const MovementComponent& Animal::getMovementComponent() const
{
	return *this->movementComponent;
}

float Animal::getMaxHp() const
{
	return this->maxHp;
}

float Animal::getHp() const
{
	return this->hp;
}

const sf::Color& Animal::getColor() const
{
	return this->body.getFillColor();
}

const CrappyNeuralNets::NeuralNet& Animal::getBrain() const
{
	return this->movementComponent->getBrain();
}

bool Animal::isCovered(const sf::Vector2f& mouse_pos_view) const
{
	float acceleration = this->movementComponent->get_x() - mouse_pos_view.x;
	float b = this->movementComponent->get_y() - mouse_pos_view.y;

	float distance = sqrt(pow(acceleration, 2) + pow(b, 2));

	return this->getRadius() >= distance;
}

float Animal::getValueOfVelocityVector() const
{
	sf::Vector2f velocity = this->movementComponent->getVelocity();

	return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}

// mutators:
void Animal::setPosition(const sf::Vector2f& new_position)
{
	this->movementComponent->set_x(new_position.x);
	this->movementComponent->set_y(new_position.y);
}

void Animal::setVelocity(const sf::Vector2f& new_velocity)
{
	this->movementComponent->set_vx(new_velocity.x);
	this->movementComponent->set_vy(new_velocity.y);
}

void Animal::setHpBarIsRendered(bool hp_bar_is_rendered)
{
	this->hpBarIsRendered = hp_bar_is_rendered;
}

void Animal::setBrainIsRendered(bool brain_is_rendered)
{
	this->brainIsRendered = brain_is_rendered;
}

void Animal::setHp(float hp)
{
	if (hp > this->maxHp)
	{
		std::cerr << "ERROR::Animal::setHp::hp ARGUMENT CANNOT BE GREATER THAN THE MAX ANIMAL HP\n";
		exit(-1);
	}

	this->hp = hp;
}

void Animal::increaseHp(float hp_increase)
{
	if (this->hp + hp_increase > this->maxHp)
	{
		std::cerr << "ERROR::Animal::setHp::hp CANNOT BE INCREASED TO A VALUE GREATER THAN THE MAX ANIMAL HP\n";
		exit(-1);
	}

	this->hp += hp_increase;
}

void Animal::setColor(const sf::Color& color)
{
	this->body.setFillColor(color);
}

void Animal::randomMutate(const CrappyNeuralNets::Scalar& mutation_percentage)
{
	this->movementComponent->randomMutate(mutation_percentage);
}

// other public methods:
void Animal::update(float dt, float speed_factor, const std::vector<double>& brain_inputs)
{
	this->movementComponent->update(dt, speed_factor, brain_inputs);

	this->body.setPosition(this->movementComponent->get_x(), this->movementComponent->get_y());

	this->updateHp(dt, speed_factor);

	this->alive = this->hp > 0.f;

	this->updateHpBar();

	this->brainPreview->update(this->movementComponent->getPosition());
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

// private initialization:
void Animal::initBody()
{
	this->body.setFillColor(sf::Color::Red);
	this->body.setPointCount(16);
	this->body.setRadius(8.f);
	this->body.setOrigin(this->body.getRadius(), this->body.getRadius());
}

void Animal::initMovementComponent()
{
	this->movementComponent = new MovementComponent();
}

void Animal::initHpBar()
{
	this->hpBar = new ProgressBar(
		sf::Vector2f(
			this->movementComponent->getPosition().x - 4.f * this->body.getRadius(),
			this->movementComponent->getPosition().y - 3.f * this->body.getRadius()
		),
		sf::Vector2f(
			8.f * this->body.getRadius(),
			this->body.getRadius()
		),
		sf::Vector2f(0.f, this->maxHp), this->maxHp,
		sf::Color(128, 128, 128), this->body.getFillColor(),
		true
	);
}

void Animal::initBrainPreview()
{
	this->brainPreview = new NeuralNetPreview(
		this->movementComponent->getBrain(),
		this->movementComponent->getPosition(),
		sf::Vector2f(144.f, 144.f),
		sf::Color(100, 100, 100)
	);
}

// private utilities:
void Animal::updateHp(float dt, float speed_factor)
{
	// some physics stuff: 

	// Pythagorean theorem for acceleration and velocity vectors:
	float acceleration = sqrt(pow(this->movementComponent->get_ax(), 2) + pow(this->movementComponent->get_ay(), 2));
	float velocity = this->getValueOfVelocityVector();
	velocity *= speed_factor;

	// calculate energy delta (where does it come from is explaneid at the bottom of the function)
	float dE = acceleration * velocity * dt;

	this->hp -= dE;

	/*
	dE <==> energy delta [J]
	P <==> power [W]
	dt <==> detla time [s]
	W <==> work [J]
	F <==> force [N]
	ds <==> delta displacement [m]
	v <==> velocity [m/s]
	m <==> mass [kg]
	a <==> acceleration [m/s^2]

	= dE
	= P(t) * dt
	= (W / dt) * dt
	= (F(t) * ds / dt) * dt
	= F(t) * v(t) * dt
	= m * a(t) * v(t) * dt
	*/
}

void Animal::updateHpBar()
{
	this->hpBar->setPosition(
		sf::Vector2f(
			this->movementComponent->getPosition().x - 4.f * this->body.getRadius(),
			this->movementComponent->getPosition().y - 3.f * this->body.getRadius()
		)
	);
	this->hpBar->setValue(this->hp);
}
