#pragma once

#include "config.h"

namespace Blueberry
{
	inline Scalar abs(const Scalar& x) { return std::abs(x); }

	inline Scalar fastSigmoid(const Scalar& x) { return x / (1.0 + std::abs(x)); }
	
	inline Scalar gaussian(const Scalar& x) { return std::exp(-(x * x)); }

	inline Scalar leakyRelu(const Scalar& x, const Scalar& alfa) { return std::max(x, x * alfa); }

	inline Scalar linear(const Scalar& x) { return x; }

	inline Scalar relu(const Scalar& x) { return std::max(x, 0.0); }
	
	inline Scalar sigmoid(const Scalar& x) { return 1.0 / (1.0 + std::exp(-x)); }

	inline Scalar sin(const Scalar& x) { return std::sin(x); }

	inline Scalar softplus(const Scalar& x) { return std::log(1.0 + std::exp(x)); }

	inline Scalar square(const Scalar& x) { return x * x; }
	
	void tanh(const Scalar& x, Scalar& y);
}
