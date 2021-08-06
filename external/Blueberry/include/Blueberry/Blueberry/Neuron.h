#pragma once

#include "backend.h"
#include "RandomEngine.h"

namespace Blueberry
{
	class Neuron
	{
	public:
		Neuron(
			bool removed = false,
			const Scalar& val = 0.0, 
			const Scalar& bias = 0.0, 
			const Scalar& actVal = 0.0,
			const char* actFunc = "linear"
		);

		std::string toStr() const;

		// accessors:

		inline bool removed() const { return m_removed; }

		inline const Scalar& getVal() const { return m_val; }
		inline const Scalar& getBias() const { return m_bias; }
		inline const Scalar& getActVal() const { return m_actVal; }

		inline const std::string& getActFunc() const { return m_actFunc; }

		// mutators:
		
		inline void remove(bool removed = true) { m_removed = removed; }

		inline void setVal(const Scalar& val) { m_val = val; }
		inline void resetVal() { setVal(0.0); }

		inline void setBias(const Scalar& bias) { m_bias = bias; }
		void mutateBias(const Scalar& maxMutationValue);

		void activate();
		inline void setActVal(const Scalar& actVal) { m_actVal = actVal; }

		inline void setActFunc(const char* actFunc) { m_actFunc = actFunc; }

		void setRandomActFunc();

	private:
		int m_removed{ false };

		Scalar m_val{ 0.0 };
		Scalar m_bias{ 0.0 };
		Scalar m_actVal{ 0.0 };

		std::string m_actFunc{ "no act func has been set yet" };
	};
}
