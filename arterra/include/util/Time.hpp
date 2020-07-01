#pragma once

#include "PCH.hpp"

namespace arterra {

	class Time {

	public:
		// Calculate time delta between last frame.
		static void CalculateDeltaTime();

		// Get the last time difference between frames.
		static inline const float GetDeltaTime() { return _deltaTime; }

	private:
		// Cache of deltaTime to stop calculating it every time its needed.
		static float _deltaTime;
		// The clock time at the last time of calculating deltaTime.
		static std::chrono::high_resolution_clock::time_point _previousTime;
	};

}