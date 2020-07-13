#pragma once

#include "PCH.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "util/DataObject.hpp"

namespace arterra {

	class KeyBind : public DataObject {
	public:
		KeyBind();

		void Update(float deltaTime);
		void CallSubscribers();
		void AddSubscriber(std::function<void(float_t)> subscriber);

		// Logging method
		virtual void DumpToLog(std::string title = "KeyBind") override;

	private:
		sf::Keyboard::Key _keyCode;
		// Whether the key is currently pressed down or not.
		bool _active = false;
		// Time the key has been held in seconds.
		float_t _timeHeld = 0.0f;
		// Callback functions to fire when the key becomes active.
		std::vector<std::function<void(float_t)>> _subscribers {};
	};

	class Input {

	public:
		Input(sf::Event &windowEvent);
		// Update tracked information for active keys.
		void Update(float_t deltaTime);
		// Register keybind subscriber.
		void RegisterKeySubscriber(const sf::Keyboard::Key key, const std::function<void(float_t)> callback);

		// Debug method
		void PrintInformation();

	private:
		// List of currently pressed down keys,
		// sf::Keyboard::Key = keycode, KeyBind = properties for this key.
		std::unordered_map<sf::Keyboard::Key, KeyBind> _pressedKeys;
		sf::Event *_event;
	};

}