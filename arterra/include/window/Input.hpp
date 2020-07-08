#pragma once

#include "PCH.hpp"
#include <GLFW/glfw3.h>

#include "util/DataObject.hpp"

namespace arterra {

	struct KeyBind : public DataObject {

		// Name of the key which this bind it linked to.
		std::string _name = "";
		// Whether the key is currently pressed down or not.
		bool _active = false;
		// Time the key has been held in seconds.
		float_t _timeHeld = 0.0f;
		// Callback functions to fire when the key becomes active.
		std::vector<std::function<void(float_t)>> _subscribers {};

		// Call all subscribers to this keybind.
		void CallSubscribers()
		{
			for (auto i : _subscribers) {
				i(_timeHeld);
			}
		}

		// Logging method.
		virtual void DumpToLog(std::string title = "KeyBind") override
		{
			Logger::Get().Log("\t", title, " - name: ", _name, "; active:", _active, "; time held:", _timeHeld,
				"; sub count: ", _subscribers.size());
		}

		// Serialise method.
		virtual std::vector<uint8_t> Serialize() override
		{
			std::vector<uint8_t> out;
			for (auto c : _name) {
				out.push_back(c);
			}
			return out;
		}
	};

	class Input {

	public:
		// Callback when a key is pressed.
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		// Update tracked information for active keys.
		static void Update(float_t deltaTime);
		// Register keybind subscriber.
		static void RegisterKeySubscriber(const int key, const std::function<void(float_t)> callback);

		// Debug method
		static void PrintInformation();

	private:
		// List of currently pressed down keys,
		// int64_t = keycode, KeyBind = properties for this key.
		static std::unordered_map<int, KeyBind> _pressedKeys;
	};

}