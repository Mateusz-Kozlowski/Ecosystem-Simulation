#include "../pch.h"
#include "InputLayer.h"

using namespace CrappyNeuralNets;

// constructor:
InputLayer::InputLayer(unsigned inputs_count, const Scalar& dropout_rate)
{
	assert(dropout_rate > 0.0 && dropout_rate <= 1.0);

	this->dropoutRate = dropout_rate;

	this->v.resize(inputs_count);
}

// mutators:
void InputLayer::setDropoutRate(const Scalar& dropout_rate)
{
	assert(dropout_rate > 0.0 && dropout_rate <= 1.0);

	this->dropoutRate = dropout_rate;
}

void InputLayer::input(const std::vector<Scalar>& inputs)
{
	this->v = inputs;

	this->dropout();
}

// accessors:
const Scalar& InputLayer::getDropoutRate() const
{
	return this->dropoutRate;
}

unsigned InputLayer::getSize() const
{
	return this->v.size();
}

const std::vector<Scalar>& InputLayer::output() const
{
	return this->v;
}

// private methods:
void InputLayer::dropout()
{
	unsigned alreadyDroppedOut = 0U;

	std::vector<bool> isDroppedOut(this->v.size());

	while (alreadyDroppedOut < this->v.size() * (1.0 - this->dropoutRate))
	{
		std::pair<unsigned, unsigned> range = { 0U, this->v.size() - 1 };

		unsigned randomIndex = RandomNumbersGenerator::getRandomNumber(range);

		if (isDroppedOut[randomIndex]) continue;

		this->v[randomIndex] = 0.0;
		isDroppedOut[randomIndex] = true;
		alreadyDroppedOut++;
	}
}
