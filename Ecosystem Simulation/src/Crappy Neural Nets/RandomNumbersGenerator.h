#pragma once

#include "Config.h"

namespace CrappyNeuralNets
{
	class RandomNumbersGenerator
	{
	public:
		static const Scalar& getRandomNumber(const std::pair<Scalar, Scalar>& range);

		static unsigned getRandomNumber(const std::pair<unsigned, unsigned>& range);
	};
}
