#pragma once

#include "RandomNumbersGenerator.h"

namespace CrappyNeuralNets
{
	class TempNet
	{
	public:
		TempNet(unsigned inputs_count, unsigned outputs_count);

		void input(const std::vector<Scalar>& input);

		const std::vector<Scalar>& output();

		void printWeights() const;

	private:
		std::vector<Scalar> m_input;
		std::vector<Scalar> m_output;

		std::vector<std::vector<Scalar>> weights;

		// private methods:
		// initialization:
		void initWeights();

		// private utilities:
		void propagateForward();
	};
}
