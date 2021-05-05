#pragma once

#include "RandomNumbersGenerator.h"

namespace CrappyNeuralNets
{
	class Matrix
	{
	public:
		// constructor:
		Matrix(const std::pair<unsigned, unsigned>& dimensions, const Scalar& initial_values = 0.0);

		// mutators:
		void setValues(const Scalar& values = 0.0);

		void setRandomValues(const std::pair<Scalar, Scalar>& range, const RandomNumbersGenerator& generator);

		void setValue(const std::pair<unsigned, unsigned>& coordinates, const Scalar& value = 0.0);

		void setRandomValue(
			const std::pair<unsigned, unsigned>& coordinates,
			const std::pair<Scalar, Scalar>& range,
			const RandomNumbersGenerator& generator
		);

		void transpose();

		// accessors:
		const Scalar& getValue(const std::pair<unsigned, unsigned>& coordinates) const;

		const std::vector<std::vector<Scalar>>& getValues() const;

		const std::pair<unsigned, unsigned>& getDimensions() const;

		const std::string& toString();

	private:
		std::vector<std::vector<Scalar>> values;

		std::pair<unsigned, unsigned> dimensions;

		std::string string;
	};
}
