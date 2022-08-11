#include "Neuron.h"

Blueberry::Neuron::Neuron(
	bool disabled,
	const Scalar& val, 
	const Scalar& bias, 
	const Scalar& actVal,
	bool enableLinearAsRandomActFunc,
	const std::string& actFunc,
	const std::string& additionalInfo)
	: m_disabled(disabled)
	, m_val(val)
	, m_bias(bias)
	, m_actVal(actVal)
	, m_enableLinearAsRandomActFunc(enableLinearAsRandomActFunc)
	, m_actFunc(actFunc)
	, m_additionalInfo(additionalInfo)
{
	
}

std::string Blueberry::Neuron::toStr() const
{
	std::stringstream ss;

	ss << m_disabled << '\n';
	ss << m_val << '\n';
	ss << m_bias << '\n';
	ss << m_actVal << '\n';
	ss << m_enableLinearAsRandomActFunc << '\n';
	ss << m_actFunc << '\n';
	ss << m_additionalInfo;

	return ss.str();
}

// mutators:

void Blueberry::Neuron::mutateBias(const Scalar& maxMutationValue)
{
	// TODO: throw an exception if maxMutationValue is smaller than 0
	// TODO: update Neuron class tests after adding that exception

	m_bias += RandomEngine::getScalarInRange(
		-maxMutationValue,
		maxMutationValue
	);
}

void Blueberry::Neuron::activate()
{
	if (m_actFunc == "abs")
	{
		m_actVal = Blueberry::abs(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "abs; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "fast sigmoid")
	{
		m_actVal = fastSigmoid(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "fast; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "gaussian")
	{
		m_actVal = gaussian(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "gaus; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "linear")
	{
		m_actVal = linear(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "linear; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "relu")
	{
		m_actVal = relu(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "relu; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}
	
	if (m_actFunc == "sigmoid")
	{
		m_actVal = sigmoid(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "sigmoid; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "sin")
	{
		m_actVal = Blueberry::sin(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "sin; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "softplus")
	{
		m_actVal = softplus(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "softplus; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "square")
	{
		m_actVal = square(m_val + m_bias);
		if (isnan(m_actVal))
		{
			std::cerr << "square; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	if (m_actFunc == "tanh")
	{
		Blueberry::tanh(m_val + m_bias, m_actVal);
		if (isnan(m_actVal))
		{
			std::cerr << "tanh; val:" << m_val << ", bias:" << m_bias << '\n';
		}
		return;
	}

	std::cerr
		<< "Error::Blueberry::Neuron::activate():\n"
		<< "invalid activation function: " << m_actFunc << '\n';
	exit(-13);
}

void Blueberry::Neuron::setRandomActFunc()
{
	do
	{
		const unsigned randomNumber = Blueberry::RandomEngine::getIntInRange(0, 9);

		switch (randomNumber)
		{
		case 0U:
			m_actFunc = "abs";
			break;
		case 1U:
			m_actFunc = "fast sigmoid";
			break;
		case 2U:
			m_actFunc = "gaussian";
			break;
		case 3U:
			m_actFunc = "linear";
			break;
		case 4U:
			m_actFunc = "relu";
			break;
		case 5U:
			m_actFunc = "sigmoid";
			break;
		case 6U:
			m_actFunc = "sin";
			break;
		case 7U:
			m_actFunc = "softplus";
			break;
		case 8U:
			m_actFunc = "square";
			break;
		case 9U:
			m_actFunc = "tanh";
			return;
		default:
			std::cerr
				<< "Error::Blueberry::Neuron::setRandomActFunc():\n"
				<< "a random number is out of range\n";
			exit(-13);
		}
	} while (m_actFunc == "linear" && !m_enableLinearAsRandomActFunc);
}

void Blueberry::Neuron::setAdditionalInfo(const std::string& additionalInfo)
{
	m_additionalInfo = additionalInfo;
}
