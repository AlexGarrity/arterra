#pragma once

#include "PCH.hpp"

#include "window/Window.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "window/Bind.hpp"

namespace arterra {

	enum MouseAxis {
		Horizontal = 0,
		Vertical = 1,
	};

	class Input {

	public:
		Input(sf::Event& windowEvent, Window* window);
		// Update keybind data.
		void Update(float deltaTime);
		// Polling methods for keybinds.
		void RegisterKeyBind(const std::string identifier, sf::Keyboard::Key key);
		KeyBindData PollKeyBind(const std::string identifier);
		// Polling methods for mousebinds.
		void RegisterMouseBind(const std::string identifier, sf::Mouse::Button button);
		MouseBindData PollMouseBind(const std::string identifier);
		// Polling methods for mouse axis.
		MouseAxisData PollMouseAxis(MouseAxis axis);

		// Set the sensitivity of a mouse axis, where sensitivity is a percentage as an integer
		void SetMouseSensitivity(MouseAxis axis, float sensitivity);

		// Lock/unlock the cursor to the window
		void SetLockCursor(bool lock);

	private:
		// List of current keybinds
		// sf::Keyboard::Key = keycode, KeyBind = properties for this bind.
		std::unordered_map<std::string, KeyBind> _keyBinds;
		std::unordered_map<std::string, MouseBind> _mouseBinds;

		// Stores the mouse movement per frame.
		sf::Vector2f _mouseDelta { 0, 0 };
		bool _cursorLocked { false };
		sf::Vector2f _inputSensitivity { 100.0f, 100.0f };

		sf::Event* _event;
		Window* _window;
	};

}