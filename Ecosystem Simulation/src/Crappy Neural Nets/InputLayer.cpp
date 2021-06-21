#include "../stdafx.h"
#include "InputLayer.h"

using namespace CrappyNeuralNets;

// constructor:
InputLayer::InputLayer(unsigned inputs_count, const Scalar& dropout_rate)
{
	assert(dropout_rate > 0.0 && dropout_rate <= 1.0);

	this->dropoutRate = dropout_rate;
	
	this->velocity.resize(inputs_count);
}

// mutators:
void InputLayer::setDropoutRate(const Scalar& dropout_rate)
{
	assert(dropout_rate > 0.0 && dropout_rate <= 1.0);

	this->dropoutRate = dropout_rate;
}

void InputLayer::input(const std::vector<Scalar>& inputs)
{
	this->velocity = inputs;

	this->dropout();
}

// accessors:
const Scalar& InputLayer::getDropoutRate() const
{
	return this->dropoutRate;
}

unsigned InputLayer::getSize() const
{
	return this->velocity.size();
}

const std::vector<Scalar>& InputLayer::output() const
{
	return this->velocity;
}

// private methods:
void InputLayer::dropout()
{
	unsigned alreadyDroppedOut = 0U;

	std::vector<bool> isDroppedOut(this->velocity.size());

	while (alreadyDroppedOut < this->velocity.size() * (1.0 - this->dropoutRate))
	{
		std::pair<unsigned, unsigned> range = { 0U, this->velocity.size() - 1 };

		unsigned randomIndex = RandomNumbersGenerator::getRandomNumber(range);

		if (isDroppedOut[randomIndex]) continue;

		this->velocity[randomIndex] = 0.0;
		isDroppedOut[randomIndex] = true;
		alreadyDroppedOut++;
	}
}
