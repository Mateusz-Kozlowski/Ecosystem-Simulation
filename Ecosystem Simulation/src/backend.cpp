#include "backend.h"

void Blueberry::tanh(const Scalar& x, Scalar& y)
{
	Blueberry::Scalar temp = exp(2 * x);
	y = (temp - 1.0) / (temp + 1.0);
}
