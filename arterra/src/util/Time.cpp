#include <util/Time.hpp>

using namespace std::chrono;

namespace arterra {

	// Re-declerations of static members.
	float Time::_deltaTime { 0.016f };
	high_resolution_clock::time_point Time::_previousTime;

	void Time::CalculateDeltaTime()
	{
		// Get the current time and subtract from the previous time when this function
		// was last run to calculate the delta.
		auto now = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(now - _previousTime).count();
		// Divide by 1000 for the time in seconds.
		_deltaTime = static_cast<float>(duration) * 0.001f;

		// Set current time to previous for the next calculation.
		_previousTime = now;
	}

}
