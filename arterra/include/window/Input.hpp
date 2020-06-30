#include "PCH.hpp"
#include <GLFW/glfw3.h>

namespace arterra {
	
	struct KeyBind {
		
		// Keybind properties
		std::string _name = "";
		bool _active = false;
		float _timeHeld = 0.0f;
		std::vector<std::function<void(float)>> _subscribers {};
		
		// Call all subscribers to this keybind
		void CallSubscribers() {
			for (auto i: _subscribers) {
				i(_timeHeld);
			}
		}
		
	};
	
	class Input {
		
		public:
			// Callback when a key is pressed
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			// Update information for active keys
			static void Update();
			// Register keybind subscriber
			static void RegisterKeySubscriber(int key, std::function<void(float)> callback);
			
			// Debug method
			static void PrintInformation();
			
		private:
			// List of currently pressed down keys
			// int = keycode, KeyBind = properties for this key
			static std::unordered_map<int, KeyBind> _pressedKeys;
			
			
	};
	
}