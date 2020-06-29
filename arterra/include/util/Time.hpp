#include <chrono>

namespace arterra {
	
	class Time {
		
		public:
			// Calculate time delta
			static void CalculateDeltaTime();
			
			// Inline getters
			static inline float GetDeltaTime() { return _deltaTime; }
			
		private:
			static float _deltaTime;
			static std::chrono::high_resolution_clock::time_point _previousTime;
			
	};
	
}