#include "bbpch.h"
#include "RandomEngine.h"

bool Blueberry::RandomEngine::s_initialized = false;
std::default_random_engine Blueberry::RandomEngine::s_eng;

// public methods:

int Blueberry::RandomEngine::getIntInRange(int first, int last)
{
	return getNumberInRange<std::uniform_int_distribution<int>>(first, last);
}

Blueberry::Scalar Blueberry::RandomEngine::getScalarInRange(
	Blueberry::Scalar first, 
	Blueberry::Scalar last)
{
	return getNumberInRange<std::uniform_real_distribution<Blueberry::Scalar>>(first, last);
}

// private methods:

void Blueberry::RandomEngine::init()
{
	std::random_device rd;
	s_eng = std::default_random_engine(rd());
}
