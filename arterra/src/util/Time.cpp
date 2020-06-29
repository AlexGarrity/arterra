#include <util/Time.hpp>

using namespace std::chrono;

namespace arterra {
	
	float Time::_deltaTime;
	high_resolution_clock::time_point Time::_previousTime;
	
	void Time::CalculateDeltaTime() {
		// Get the current time and subtract from the previous time when this function
		// was last run.
		auto now = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(now - _previousTime).count();
		_deltaTime = static_cast<float>(duration) * 0.001; // Divide by 1000 for seconds.
		
		// Set current time to previous for the next calculation.
		_previousTime = now;
	}
	
}

