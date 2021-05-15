#pragma once
#include "PCH.hpp"

namespace arterra {

	class Random {

	public:
		// ctor
		Random();

		// Get a random integer in a specific range.
		int64_t IntInRange(int64_t min, int64_t max);
		// Get a random float in a specific range.
		float FloatInRange(float min, float max);

		// Get a completely random number with no bounds.
		long Generate();

	private:
		// The random generation engine.
		std::mt19937_64 _engine;
	};

}