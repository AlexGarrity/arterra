#include <chrono>

namespace arterra {
	
	class Time {
		
		public:	
			static float DeltaTime();
			
			static void CalculateDeltaTime();
			
		private:
			static float _deltaTime;
			static std::chrono::high_resolution_clock::time_point _previousTime;
			
	};
	
}