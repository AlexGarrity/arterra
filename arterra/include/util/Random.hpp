#include <random>

namespace arterra {

	class Random {

	public:
		// ctor
		Random(const long seed);

		// Get a random integer in a specific range.
		int IntInRange(const int min, const int max);
		// Get a random float in a specific range.
		float FloatInRange(const float min, const float max);

		// Get a completely random number with no bounds.
		long Generate();

	private:
		// The random generation engine.
		std::mt19937_64 _engine;
	};

}