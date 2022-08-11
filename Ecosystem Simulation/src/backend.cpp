#include "backend.h"

Blueberry::Scalar Blueberry::fastSigmoid(const Scalar& x)
{
	if (isinf(abs(x)))
	{
		if (x > 0.0)
		{
			return 1.0;
		}

		return -1.0;
	}
	
	return x / (1.0 + std::abs(x));
}

Blueberry::Scalar Blueberry::gaussian(const Scalar& x)
{
	if (abs(x) > sqrt(std::numeric_limits<Blueberry::Scalar>::max()))
	{
		return 0.0;
	}
	
	return std::exp(-(x * x));
}

Blueberry::Scalar Blueberry::sigmoid(const Scalar& x)
{
	if (-x > log(std::numeric_limits<Blueberry::Scalar>::max()))
	{
		return 0.0;
	}
	
	return 1.0 / (1.0 + std::exp(-x));
}

Blueberry::Scalar Blueberry::sin(const Scalar& x)
{
	if (isinf(x))
	{
		return 0.0;
	}
	
	return std::sin(x);
}

Blueberry::Scalar Blueberry::softplus(const Scalar& x)
{
	if (x > log(std::numeric_limits<Blueberry::Scalar>::max()))
	{
		return std::numeric_limits<Blueberry::Scalar>::max();
	}
	
	return std::log(1.0 + std::exp(x));
}

Blueberry::Scalar Blueberry::square(const Scalar& x)
{
	if (x > sqrt(std::numeric_limits<Blueberry::Scalar>::max()))
	{
		return std::numeric_limits<Blueberry::Scalar>::max();
	}

	return x * x;
}

void Blueberry::tanh(const Scalar& x, Scalar& y)
{
	const Blueberry::Scalar ARG = 2 * x;
	
	if (ARG > log(std::numeric_limits<Blueberry::Scalar>::max()))
	{
		y = 1.0;
		return;
	}

	if (ARG - 1.0 == -std::numeric_limits<Blueberry::Scalar>::infinity())
	{
		y = -1.0;
		return;
	}
	
	Blueberry::Scalar temp = exp(ARG);
	y = (temp - 1.0) / (temp + 1.0);
}
