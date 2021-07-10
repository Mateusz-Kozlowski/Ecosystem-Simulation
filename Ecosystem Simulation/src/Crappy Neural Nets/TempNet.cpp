#include "../stdafx.h"
#include "TempNet.h"

using namespace CrappyNeuralNets;

TempNet::TempNet(unsigned inputs_count)
{
	this->v.resize(inputs_count);

	this->initWeights();
}

void TempNet::input(const std::vector<CrappyNeuralNets::Scalar>& input)
{
	if (input.size() != this->v.size())
		throw("ERROR::TempNet::input::input SIZE IS NOT CORRECT\n");

	for (int i = 0; i < input.size(); i++)
		this->v[i] = input[i];
}

const std::vector<CrappyNeuralNets::Scalar>& TempNet::output()
{
	this->propagateForward();

	return this->v;
}

// private methods:
 
// initialization:
void TempNet::initWeights()
{
	this->weights.resize(this->v.size());

	std::pair<CrappyNeuralNets::Scalar, CrappyNeuralNets::Scalar> range(-1.0, 1.0);

	for (auto& it : this->weights)
		it = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(range);
}

// private utilities:
void CrappyNeuralNets::TempNet::propagateForward()
{
	for (int i = 0; i < this->v.size(); i++)
		this->v[i] *= this->weights[i];
}
