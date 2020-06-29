#include <random>

namespace arterra {
	
	class Random {
		
		private:
			std::mt19937_64 _engine;
			
		public:
			// ctor
			Random(long seed);
			
			// Get a random value in a specific range
			int IntInRange(int min, int max);
			float FloatInRange(float min, float max);
			
			// Just run the generator if needed
			long Generate();
			
	};
	
}