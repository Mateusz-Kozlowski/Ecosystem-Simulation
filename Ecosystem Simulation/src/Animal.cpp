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
	: brainIsRendered(false), alive(true)
{
	this->movementComponent = new MovementComponent();

	this->renderingComponent = new RenderingComponent(*this->movementComponent);

	this->radius = 8.f;

	// get rid of that hardcoded stuff:
	this->hpBar = new ProgressBar(
		this->movementComponent->get_x() - 4.f * this->radius, this->movementComponent->get_y() - 3.f * this->radius,
		8.f * this->radius, this->radius,
		sf::Vector2f(0.f, 1000.f), 1000.f, sf::Color(128, 128, 128), sf::Color::Red
	);
}

Animal::~Animal()
{
	delete this->movementComponent;
	delete this->renderingComponent;
	delete this->hpBar;
}

// initialization:
void Animal::loadFromFolder(const std::string& folder_path)
{	
	this->movementComponent->loadFromFolder(folder_path);
}

// accessors:
sf::Vector2f Animal::getPos() const
{
	return { this->movementComponent->get_x(), this->movementComponent->get_y() };
}

sf::Vector2f Animal::getVelocity() const
{
	return { this->movementComponent->get_vx(), this->movementComponent->get_vy() };
}

float Animal::getRadius() const
{
	return this->radius;
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
	return this->hpBar->getValue();
}

// mutators:
void Animal::setVelocity(const sf::Vector2f& v)
{
	this->movementComponent->set_vx(v.x);
	this->movementComponent->set_vy(v.y);
}

void Animal::setBrainIsRendered(bool brain_is_rendered)
{
	this->brainIsRendered = brain_is_rendered;
}

void Animal::setHp(float new_hp)
{
	this->hpBar->increaseValue(new_hp - this->hpBar->getValue());
}

// other public methods:
void Animal::update(float dt, const std::vector<double>& brain_inputs)
{
	this->movementComponent->update(dt, brain_inputs);

	this->renderingComponent->updateBodyRedering(*this->movementComponent);

	if (this->brainIsRendered) this->renderingComponent->updateBrainRendering(*this->movementComponent);

	this->updateHpBar(dt);

	this->alive = this->hpBar->getValue() > 0.f;
}

void Animal::renderBody(sf::RenderTarget& target)
{
	this->renderingComponent->renderBody(target);

	this->hpBar->render(target);
}

void Animal::renderBrain(sf::RenderTarget& target)
{
	if (this->brainIsRendered) this->renderingComponent->renderBrain(target);
}

// private utilities:
void Animal::updateHpBar(float dt)
{
	// first update position:
	this->hpBar->setPos(
		sf::Vector2f(
			this->movementComponent->get_x() - 4.f * this->radius,
			this->movementComponent->get_y() - 3.f * this->radius
		)
	);

	// and then update value:
	// some physics stuff: 

	// Pythagorean theorem for acceleration and velocity vectors:
	float a = sqrt(pow(this->movementComponent->get_ax(), 2) + pow(this->movementComponent->get_ay(), 2));
	float v = sqrt(pow(this->movementComponent->get_vx(), 2) + pow(this->movementComponent->get_vy(), 2));
	
	// calculate energy delta (where does it come from is explaneid at the bottom of the function)
	float dE = a * v * dt;

	//this->hpBar->increaseValue(-dE);

	this->hpBar->increaseValue(-0.05f);

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
