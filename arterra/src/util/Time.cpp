#include <util/Time.hpp>

namespace arterra {

	void Time::CalculateDeltaTime()
	{
		// Get the current time and subtract from the previous time when this function
		// was last run to calculate the delta.
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _previousTime).count();
		// Divide by 1000 for the time in seconds.
		_deltaTime = static_cast<float_t>(duration) * 0.001f;

		// Set current time to previous for the next calculation.
		_previousTime = now;
	}

}
