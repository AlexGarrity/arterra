#include <util/Random.hpp>

namespace arterra {

	Random::Random(const long seed)
	{
		// Initialise the engine with a seed.
		_engine.seed(seed);
	}

	int Random::IntInRange(const int min, const int max)
	{
		// Generate an int in the range.
		std::uniform_int_distribution<> distribution(min, max);
		return distribution(_engine);
	}

	float Random::FloatInRange(const float min, const float max)
	{
		// Generate a float in the range.
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(_engine);
	}

	long Random::Generate()
	{
		// Just run the generator.
		return static_cast<long>(_engine());
	}

}