#include "../stdafx.h"
#include "Matrix.h"

using namespace CrappyNeuralNets;

// constructor:
Matrix::Matrix(const std::pair<unsigned, unsigned>& dimensions, const Scalar& initial_values)
{
	this->values.resize(dimensions.first);
	
	for (auto& value : this->values) value.resize(dimensions.second, initial_values);

	this->dimensions = dimensions;

	this->string = "";
}

// mutators:
void Matrix::setValues(const Scalar& values)
{
	for (auto& velocity : this->values) for (auto& c : velocity) c = values;
}

void Matrix::setRandomValues(const std::pair<Scalar, Scalar>& range)
{
	for (auto& velocity : this->values) 
		for (auto& c : velocity) 
			c = RandomNumbersGenerator::getRandomNumber(range);
}

void Matrix::setValue(const std::pair<unsigned, unsigned>& coordinates, const Scalar& value)
{
	this->values[coordinates.first][coordinates.second] = value;
}

void Matrix::setRandomValue(
	const std::pair<unsigned, unsigned>& coordinates, 
	const std::pair<Scalar, Scalar>& range, 
	const RandomNumbersGenerator& generator
)
{
	this->values[coordinates.first][coordinates.second] = generator.getRandomNumber(range);
}

void Matrix::transpose()
{
	std::vector<std::vector<Scalar>> newValues(
		this->getDimensions().second,
		std::vector<Scalar>(this->getDimensions().first)
	);

	for (int i = 0; i < this->getDimensions().first; i++)
		for (int j = 0; j < this->getDimensions().second; j++) newValues[j][i] = this->values[i][j];

	this->values = newValues;

	std::swap(this->dimensions.first, this->dimensions.second);
}

// accessors:
const Scalar& Matrix::getValue(const std::pair<unsigned, unsigned>& coordinates) const
{
	return this->values[coordinates.first][coordinates.second];
}

const std::vector<std::vector<Scalar>>& Matrix::getValues() const
{
	return this->values;
}

const std::pair<unsigned, unsigned>& Matrix::getDimensions() const
{
	return this->dimensions;
}

const std::string& Matrix::toString()
{
	this->string = "";

	for (auto& velocity : this->values)
	{
		for (auto& c : velocity) this->string += std::to_string(c) + '\t';
		
		this->string += '\n';
	}

	return this->string;
}
