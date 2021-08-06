#pragma once

#include "config.h"

namespace Blueberry
{
	class RandomEngine
	{
	public:
		static int getIntInRange(int first, int last);
		static Scalar getScalarInRange(Scalar first, Scalar last);

	private:
		template <typename Dist, typename T>
		static T getNumberInRange(T low, T high);

		static void init();

	private:
		static bool s_initialized;
		static std::default_random_engine s_eng;
	};

	template<typename Dist, typename T>
	T RandomEngine::getNumberInRange(T first, T last)
	{
		if (!s_initialized)
		{
			init();
			s_initialized = true;
		}

		Dist dist(first, last);
		return dist(s_eng);
	}
}
