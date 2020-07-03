#pragma once

#include "PCH.hpp"

namespace arterra {

	class Time {

	public:
		// ctor
		Time() : _deltaTime {0.0f}, _previousTime{std::chrono::high_resolution_clock::now()} {}

		// Calculate time delta between last frame.
		void CalculateDeltaTime();

		// Get the last time difference between frames.
		inline const float GetDeltaTime() { return _deltaTime; }

	private:
		// Cache of deltaTime to stop calculating it every time its needed.
		float _deltaTime;
		// The clock time at the last time of calculating deltaTime.
		std::chrono::high_resolution_clock::time_point _previousTime;
	};

}