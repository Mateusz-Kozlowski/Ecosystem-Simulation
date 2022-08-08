	#pragma once

#include "backend.h"
#include "RandomEngine.h"

namespace Blueberry
{
	class Neuron
	{
	public:
		Neuron(
			bool disabled = false,
			const Scalar& val = 0.0, 
			const Scalar& bias = 0.0, 
			const Scalar& actVal = 0.0,
			bool enableLinearAsRandomActivationFunction = true,
			const std::string& actFunc = "linear",
			const std::string& additionalInfo = ""
		);

		std::string toStr() const;

		// accessors:

		inline bool disabled() const { return m_disabled; }

		inline const Scalar& getVal() const { return m_val; }
		inline const Scalar& getBias() const { return m_bias; }
		inline const Scalar& getActVal() const { return m_actVal; }

		inline const std::string& getActFunc() const { return m_actFunc; }

		inline const std::string& getAdditionalInfo() const { return m_additionalInfo; }

		// mutators:
		
		inline void disable(bool disabled = true) { m_disabled = disabled; }

		inline void setVal(const Scalar& val) { m_val = val; }
		inline void resetVal() { setVal(0.0); }

		inline void setBias(const Scalar& bias) { m_bias = bias; }
		void mutateBias(const Scalar& maxMutationValue);

		void activate();
		inline void setActVal(const Scalar& actVal) { m_actVal = actVal; }

		inline void enableLinearAsRandomActFunc(bool enable) { m_enableLinearAsRandomActFunc = enable; }

		inline void setActFunc(const char* actFunc) { 
			std::cout << "WHY AM I USED?\n";
			exit(-13);
			m_actFunc = actFunc; 
		}

		void setRandomActFunc();

		void setAdditionalInfo(const std::string& additionalInfo);

	private:
		int m_disabled{ false };

		Scalar m_val{ 0.0 };
		Scalar m_bias{ 0.0 };
		Scalar m_actVal{ 0.0 };

		bool m_enableLinearAsRandomActFunc;

		std::string m_actFunc{ "no act func has been set yet" };

		std::string m_additionalInfo;
	};
}
