#include "../pch.h"
#include "RandomNumbersGenerator.h"

using namespace CrappyNeuralNets;

const Scalar& RandomNumbersGenerator::getRandomNumber(const std::pair<Scalar, Scalar>& range)
{
	Scalar randomNum = static_cast<Scalar>(rand());
	Scalar randMax = static_cast<Scalar>(RAND_MAX);

	Scalar rangeSize = range.second - range.first;

	return (rangeSize * randomNum / randMax) + range.first;
}

unsigned RandomNumbersGenerator::getRandomNumber(const std::pair<unsigned, unsigned>& range)
{
	assert(range.first <= range.second);

	unsigned rangeSize = range.second - range.first;
	
	unsigned randomNum = rand() % (rangeSize + 1);

	return randomNum + range.first;
}
