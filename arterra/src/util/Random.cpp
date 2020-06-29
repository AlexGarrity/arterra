#include <util/Random.hpp>

namespace arterra {
	
	Random::Random(long seed) {
		// Initialise the engine with a seed
		_engine.seed(seed);
	}
	
	int Random::IntInRange(int min, int max) {
		std::uniform_int_distribution<> distribution(min, max);
		return distribution(_engine);
	}
	
	float Random::FloatInRange(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(_engine);
	}
	
	long Random::Generate() {
		return _engine();
	}
	
}