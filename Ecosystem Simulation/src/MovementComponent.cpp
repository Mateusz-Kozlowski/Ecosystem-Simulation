#include "pch.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent()
	: m_brain(std::make_unique<Blueberry::Brain>(5U, 2U))
	, m_prevVelocity(0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f)
	, m_acceleration(0.0f, 0.0f)
{
	m_brain->mutateRandomNeuronBias();
	m_brain->mutateRandomNeuronBias();
}

MovementComponent::MovementComponent(
	const sf::Vector2f& defaultVelocity,
	const char* brainFilePath)
	: m_brain(std::make_unique<Blueberry::Brain>(0U, 0U))
	, m_prevVelocity(0.0f, 0.0f)
	, m_velocity(defaultVelocity)
	, m_acceleration(0.0f, 0.0f)
{
	loadBrainFromFile(brainFilePath);
}

MovementComponent::MovementComponent(const MovementComponent& rhs)
	: m_brain(std::make_unique<Blueberry::Brain>(0U, 0U))
	, m_prevVelocity(0.0f, 0.0f)
	, m_velocity(rhs.m_velocity)
	, m_acceleration(rhs.m_acceleration)
{
	*m_brain = *rhs.m_brain;
}

MovementComponent& MovementComponent::operator=(const MovementComponent& rhs)
{
	if (this != &rhs)
	{
		*m_brain = *rhs.m_brain;
		m_prevVelocity = rhs.m_prevVelocity;
		m_velocity = rhs.m_velocity;
		m_acceleration = rhs.m_acceleration;
	}

	return *this;
}

void MovementComponent::saveBrainToFile(const char* filePath) const
{
	m_brain->saveToFile(filePath);
}

void MovementComponent::loadBrainFromFile(const char* filePath)
{
	m_brain->loadFromFile(filePath);
}

void MovementComponent::update(
	float dt,
	float speedFactor,
	const std::vector<double>& brainInputs)
{
	// update acceleration:	
	m_brain->propagateForward(brainInputs);

	auto brainOutput = m_brain->getOutput();

	// TODO: implement slowing down using speed_factor:

	// update acceleration:
	m_acceleration.x = brainOutput[0];
	m_acceleration.y = brainOutput[1];

	// update previous velocity:
	m_prevVelocity = m_velocity;

	// update velocity:
	m_velocity.x += m_acceleration.x * dt;
	m_velocity.y += m_acceleration.y * dt;
}

// accessors:

const Blueberry::Brain& MovementComponent::getBrain() const
{
	return *m_brain;
}

float MovementComponent::getEnergyToExpel() const
{
	if (getKineticEnergyDelta() >= 0.0f) return 0.0f;

	return -2.0f * getKineticEnergyDelta();
}

float MovementComponent::getKineticEnergyDelta() const
{
	return getKineticEnergy() - getPreviousKineticEnergy();
}

float MovementComponent::getPreviousKineticEnergy() const
{
	return 0.5f * std::pow(getPreviousVelocityVectorValue(), 2.0f);
}

float MovementComponent::getKineticEnergy() const
{
	return 0.5f * std::pow(getVelocityVectorValue(), 2.0f);
}

float MovementComponent::getPreviousVelocityVectorValue() const
{
	return getVectorValue(m_prevVelocity);
}

float MovementComponent::getVelocityVectorValue() const
{
	return getVectorValue(m_velocity);
}

float MovementComponent::getAccelerationVectorValue() const
{
	return getVectorValue(m_acceleration);
}

const sf::Vector2f& MovementComponent::getPreviousVelocityVector() const
{
	return m_prevVelocity;
}

const sf::Vector2f& MovementComponent::getVelocityVector() const
{
	return m_velocity;
}

const sf::Vector2f& MovementComponent::getAccelerationVector() const
{
	return m_acceleration;
}

// mutators:

void MovementComponent::mutateBrain(unsigned brainMutationsCount)
{
	m_brain->mutate(brainMutationsCount);
}

void MovementComponent::setVelocity(const sf::Vector2f& velocity)
{
	m_prevVelocity = m_velocity;
	m_velocity = velocity;
}

void MovementComponent::setVelocity(float x, float y)
{
	m_prevVelocity = m_velocity;
	m_velocity.x = x;
	m_velocity.y = y;
}

void MovementComponent::setVelocityX(float vx)
{
	m_prevVelocity.x = m_velocity.x;
	m_velocity.x = vx;
}

void MovementComponent::setVelocityY(float vy)
{
	m_prevVelocity.y = m_velocity.y;
	m_velocity.y = vy;
}

// private methods:

float MovementComponent::getVectorValue(const sf::Vector2f& vector)
{
	return sqrt(std::pow(vector.x, 2.0f) + std::pow(vector.y, 2.0f));
}
