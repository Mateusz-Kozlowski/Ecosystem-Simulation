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
Animal::Animal()
	: brainIsRendered(false), alive(true), defaultHpValue(10e6), hp(defaultHpValue)
{
	this->body.setFillColor(sf::Color::Red);
	this->body.setPointCount(16);
	this->body.setRadius(8.f);
	this->body.setOrigin(this->body.getRadius(), this->body.getRadius());

	this->movementComponent = new MovementComponent();

	this->brainPreview = new NeuralNetPreview(
		this->movementComponent->getBrain(),
		this->body.getPosition(),
		sf::Vector2f(144.f, 144.f),
		sf::Color(128, 128, 128, 128)
	);

	// get rid of that hardcoded stuff:
	this->hpBar = new ProgressBar(
		sf::Vector2f(
			this->movementComponent->get_x() - 4.f * this->body.getRadius(), 
			this->movementComponent->get_y() - 3.f * this->body.getRadius()
		),
		sf::Vector2f(
			8.f * this->body.getRadius(), 
			this->body.getRadius()
		),
		sf::Vector2f(0.f, this->defaultHpValue), this->defaultHpValue, 
		sf::Color(128, 128, 128), sf::Color::Red,
		true
	);
}

Animal::~Animal()
{
	delete this->movementComponent;
	delete this->brainPreview;
	delete this->hpBar;
}

// initialization:
void Animal::loadFromFolder(const std::string& folder_path)
{	
	this->movementComponent->loadFromFolder(folder_path);
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

bool Animal::isBrainRendered() const
{
	return this->brainIsRendered;
}

bool Animal::isAlive() const
{
	return this->alive;
}

float Animal::getHp() const
{
	return this->hp;
}

// mutators:
void Animal::setPosition(const sf::Vector2f& new_pos)
{
	this->movementComponent->set_x(new_pos.x);
	this->movementComponent->set_y(new_pos.y);
}

void Animal::setVelocity(const sf::Vector2f& v)
{
	this->movementComponent->set_vx(v.x);
	this->movementComponent->set_vy(v.y);
}

void Animal::setBrainIsRendered(bool brain_is_rendered)
{
	this->brainIsRendered = brain_is_rendered;
}

void Animal::setHp(float hp)
{
	std::cout << "Hp set to: " << hp << '\n';
	this->hp = hp;
	this->hpBar->setValue(hp);
}

void Animal::increaseHp(float hp_increase)
{
	this->hp += hp_increase;
	this->hpBar->increaseValue(hp_increase);
}

void Animal::setColor(const sf::Color& color)
{
	this->body.setFillColor(color);
	this->hpBar->setProgressColor(color);
}

// other public methods:
void Animal::updateBodyAndHp(float dt, float speed_factor, const std::vector<double>& brain_inputs)
{
	this->movementComponent->update(dt, speed_factor, brain_inputs);

	this->body.setPosition(this->movementComponent->get_x(), this->movementComponent->get_y());

	this->updateHp(dt, speed_factor);

	this->alive = this->hp > 0.f;
}

void Animal::updateBrainPreview()
{
	this->brainPreview->update(this->body.getPosition());
}

void Animal::renderBody(sf::RenderTarget& target) const
{
	target.draw(this->body);	
}

void Animal::renderHpBar(sf::RenderTarget& target) const
{
	this->hpBar->render(target);
}

void Animal::renderBrain(sf::RenderTarget& target) const
{
	this->brainPreview->render(target);
}

bool Animal::isCovered(const sf::Vector2f& mouse_pos_view) const
{	
	float a = this->movementComponent->get_x() - mouse_pos_view.x;
	float b = this->movementComponent->get_y() - mouse_pos_view.y;

	float distance = sqrt(pow(a, 2) + pow(b, 2));

	return this->getRadius() >= distance;
}

// private utilities:
void Animal::updateHp(float dt, float speed_factor)
{
	// first update position:
	this->hpBar->setPosition(
		sf::Vector2f(
			this->movementComponent->get_x() - 4.f * this->body.getRadius(),
			this->movementComponent->get_y() - 3.f * this->body.getRadius()
		)
	);

	// and then update value:
	// some physics stuff: 

	// Pythagorean theorem for acceleration and velocity vectors:
	float a = sqrt(pow(this->movementComponent->get_ax(), 2) + pow(this->movementComponent->get_ay(), 2));
	float v = sqrt(pow(this->movementComponent->get_vx(), 2) + pow(this->movementComponent->get_vy(), 2));
	v *= speed_factor;

	// calculate energy delta (where does it come from is explaneid at the bottom of the function)
	float dE = -a * v * dt;

	//this->increaseHp(dE);
	this->hp += dE;
	this->hpBar->increaseValue(dE);

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
