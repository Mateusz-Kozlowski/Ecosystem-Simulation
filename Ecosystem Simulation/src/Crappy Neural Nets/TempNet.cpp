#include "../stdafx.h"
#include "TempNet.h"

using namespace CrappyNeuralNets;

TempNet::TempNet(unsigned inputs_count, unsigned outputs_count)
{
	this->m_input.resize(inputs_count);
	this->m_output.resize(outputs_count);

	this->initWeights();
}

void TempNet::input(const std::vector<CrappyNeuralNets::Scalar>& input)
{
	if (input.size() != this->m_input.size())
		throw("ERROR::TempNet::input::input SIZE IS NOT CORRECT\n");
	
	this->m_input = input;
}

const std::vector<CrappyNeuralNets::Scalar>& TempNet::output()
{
	this->propagateForward();

	return this->m_output;
}

void CrappyNeuralNets::TempNet::printWeights() const
{
	for (const auto& v : this->weights)
	{
		for (const auto& it : v)
			std::cout << it << ' ';
		std::cout << '\n';
	}
}

// private methods:
 
// initialization:
void TempNet::initWeights()
{
	this->weights.resize(this->m_output.size());

	for (int i = 0; i < this->weights.size(); i++)
		this->weights[i].resize(this->m_input.size());

	std::pair<CrappyNeuralNets::Scalar, CrappyNeuralNets::Scalar> range(-1.0, 1.0);

	for (auto& v : this->weights)
		for(auto& it : v)
			it = CrappyNeuralNets::RandomNumbersGenerator::getRandomNumber(range);
}

// private utilities:
void CrappyNeuralNets::TempNet::propagateForward()
{
	for (auto& it : this->m_output)
		it = 0.0;

	for (int i = 0; i < this->weights.size(); i++)
		for (int j = 0; j < this->weights[i].size(); j++)
			this->m_output[i] += this->m_input[j] * this->weights[i][j];
}
