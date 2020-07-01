#include "window/Input.hpp"

namespace arterra {

	// Re-declerations of static members.
	std::unordered_map<int, KeyBind> Input::_pressedKeys;

	void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action) {
			case GLFW_PRESS:
				// Triggers in the frame when the key is initially pressed down.
				// The key may not exist, so it is created here and given a name.
				// The state is set to active.
				{
					_pressedKeys[key]._active = true;
					_pressedKeys[key]._name = "unknown";
					auto keyName = glfwGetKeyName(key, scancode);
					if (keyName)
						_pressedKeys[key]._name = keyName;
				}
				break;
			case GLFW_RELEASE:
				// Triggers in the frame when the key is released.
				// Set the key to be inactive, and reset its held time.
				_pressedKeys[key]._active = false;
				_pressedKeys[key]._timeHeld = 0.0f;
				break;
		}
	}

	void Input::Update()
	{
		// If no keys are being tracked, return quickly.
		if (_pressedKeys.empty()) {
			return;
		}

		// Increase the hold time for all actively held keys.
		for (auto& i : _pressedKeys) {
			if (i.second._active) {
				i.second._timeHeld += Time::GetDeltaTime();
				i.second.CallSubscribers();
			}
		}

		// DEBUG
		// PrintInformation();
	}

	void Input::RegisterKeySubscriber(const int key, const std::function<void(float)> callback)
	{
		// Add the callback function to the key subscription.
		_pressedKeys[key]._subscribers.emplace_back(callback);
	}

	void Input::PrintInformation()
	{
		// Log the pressed keys information.
		Logger::Get().Log(Logger::Debug, "===================");
		Logger::Get().Log(Logger::Debug, " KEY INPUTS STORED ");
		for (auto& i : _pressedKeys) {
			i.second.DumpToLog();
		}
		Logger::Get().Log(Logger::Debug, "===================");
	}

}