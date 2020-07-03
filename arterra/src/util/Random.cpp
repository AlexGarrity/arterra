#include <util/Random.hpp>

namespace arterra {

	Random::Random(const long seed)
	{
		// Initialise the engine with a seed.
		_engine.seed(seed);
	}

	int64_t Random::IntInRange(const int64_t min, const int64_t max)
	{
		// Generate an integer in the range.
		std::uniform_int_distribution<int64_t> distribution(min, max);
		return distribution(_engine);
	}

	float_t Random::FloatInRange(const float_t min, const float_t max)
	{
		// Generate a float_tin the range.
		std::uniform_real_distribution<float_t> distribution(min, max);
		return distribution(_engine);
	}

	long Random::Generate()
	{
		// Just run the generator.
		return static_cast<long>(_engine());
	}

}