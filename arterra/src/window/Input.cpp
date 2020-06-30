#include "window/Input.hpp"


namespace arterra {
	
	std::unordered_map<int, KeyBind> Input::_pressedKeys;
	
	void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
		switch (action)
		{
		case GLFW_PRESS:
			// Triggers on frame on initial press down
			// The key may not exist, so it is created here and given a name
			// Set the key to active state
			{
				_pressedKeys[key]._active = true;
				_pressedKeys[key]._name = "unknown";
				auto keyName = glfwGetKeyName(key, scancode);
				if (keyName) {
					_pressedKeys[key]._name = keyName;
				}
			}
			break;
		case GLFW_RELEASE:
			// Triggers on frame on initial key release
			// Set the key to inactive state, and reset held time
			_pressedKeys[key]._active = false;
			_pressedKeys[key]._timeHeld = 0.0f;
			break;
		default:
			break;
		}
		
	}
	
	void Input::Update() {
		// If no keys are being tracked, return quickly
		if (_pressedKeys.empty()) { return; }
		
		// Increase hold time for all actively held keys
		for (auto& i: _pressedKeys) {
			if (i.second._active) {
				i.second._timeHeld += Time::GetDeltaTime();
				i.second.CallSubscribers();
			}
		}
		
		// DEBUG
		PrintInformation();
	}
	
	void Input::RegisterKeySubscriber(int key, std::function<void(float)> callback) {
		// Add the callback to the key subscription
		_pressedKeys[key]._subscribers.emplace_back(callback);
	}
	
	void Input::PrintInformation() {
		Logger::Get().Log(Logger::Debug, "===================");
		Logger::Get().Log(Logger::Debug, " KEY INPUTS STORED ");
		
		for (auto& i: _pressedKeys) {
			std::cout << "Key: " << i.first << " n: " << i.second._name << " t: " << i.second._timeHeld
			<< " a: " << i.second._active << std::endl;
		}
		
		Logger::Get().Log(Logger::Debug, "===================");
	}
	
}