#pragma once

#include "RandomNumbersGenerator.h"

namespace CrappyNeuralNets
{
	class TempNet
	{
	public:
		TempNet(unsigned inputs_count);

		void input(const std::vector<CrappyNeuralNets::Scalar>& input);

		const std::vector<CrappyNeuralNets::Scalar>& output();

	private:
		std::vector<CrappyNeuralNets::Scalar> v;

		std::vector<CrappyNeuralNets::Scalar> weights;

		// private methods:
		// initialization:
		void initWeights();

		// private utilities:
		void propagateForward();
	};
}
