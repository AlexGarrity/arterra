#include <random>

namespace arterra {

	class Random {

	public:
		// ctor
		Random(const long seed);

		// Get a random integer in a specific range.
		int64_t IntInRange(const int64_t min, const int64_t max);
		// Get a random float_tin a specific range.
		float_t FloatInRange(const float_t min, const float_t max);

		// Get a completely random number with no bounds.
		long Generate();

	private:
		// The random generation engine.
		std::mt19937_64 _engine;
	};

}