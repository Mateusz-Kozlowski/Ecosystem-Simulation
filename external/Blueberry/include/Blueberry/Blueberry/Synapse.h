#pragma once

#include "config.h"
#include "RandomEngine.h"

namespace Blueberry
{
	class Synapse
	{
	public:
		Synapse(
			bool removed,
			unsigned src, 
			unsigned dst, 
			const Scalar& weight
		);

		std::string toStr() const;

		// accessors:

		inline bool removed() const { return m_removed; }

		inline unsigned getSrc() const { return m_src; }
		inline unsigned getDst() const { return m_dst; }
		
		inline const Scalar& getWeight() const { return m_weight; }

		// mutators:

		inline void remove(bool removed = true) { m_removed = removed; }

		inline void setSrc(unsigned src) { m_src = src; }
		inline void setDst(unsigned dst) { m_dst = dst; }

		void mutateWeight(const Scalar& maxMutationValue);

		inline void setWeight(const Scalar& weight) { m_weight = weight; }

	private:
		bool m_removed{ false };

		unsigned m_src{ 0U };
		unsigned m_dst{ 0U };

		Scalar m_weight{ 1.0 };
	};
}
