#include "Synapse.h"

Blueberry::Synapse::Synapse(
	bool disabled,
	unsigned src, 
	unsigned dst, 
	const Scalar& weight)
	: m_disabled(disabled)
	, m_src(src)
	, m_dst(dst)
	, m_weight(weight)
{
	
}

std::string Blueberry::Synapse::toStr() const
{
	std::stringstream ss;

	ss << m_disabled << '\n';
	ss << m_src << '\n';
	ss << m_dst << '\n';
	ss << m_weight;

	return ss.str();
}

void Blueberry::Synapse::mutateWeight(const Scalar& maxMutationValue)
{
	m_weight += RandomEngine::getScalarInRange(
		-maxMutationValue,
		maxMutationValue
	);
}
