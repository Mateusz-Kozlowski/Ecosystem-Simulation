#pragma once

#include "Config.h"

namespace CrappyNeuralNets
{
	class RandomNumbersGenerator
	{
	public:
		// constructor:
		RandomNumbersGenerator();

		// accessors:
		const Scalar& getRandomNumber(const std::pair<Scalar, Scalar>& range) const;

		unsigned getRandomNumber(const std::pair<unsigned, unsigned>& range) const;
	};
}
