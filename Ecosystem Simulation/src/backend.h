#pragma once

#include "config.h"

namespace Blueberry
{
	inline Scalar abs(const Scalar& x) { return std::abs(x); }

	Scalar fastSigmoid(const Scalar& x);
	
	Scalar gaussian(const Scalar& x);

	inline Scalar linear(const Scalar& x) { return x; }

	inline Scalar relu(const Scalar& x) { return std::max(x, 0.0); }
	
	Scalar sigmoid(const Scalar& x);

	Scalar sin(const Scalar& x);

	Scalar softplus(const Scalar& x);

	Scalar square(const Scalar& x);
	
	void tanh(const Scalar& x, Scalar& y);
}
