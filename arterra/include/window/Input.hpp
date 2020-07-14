#pragma once

#include "PCH.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "window/Bind.hpp"

namespace arterra {

	class Input {

	public:
		Input(sf::Event &windowEvent);
		// Update keybind data.
		void Update(float_t deltaTime);
		// Polling methods for keybinds.
		void RegisterKeyBind(const std::string identifier, sf::Keyboard::Key key);
		KeyBindData PollKeyBind(const std::string identifier);
		// Polling methods for mousebinds.
		void RegisterMouseBind(const std::string identifier, sf::Mouse::Button button);
		MouseBindData PollMouseBind(const std::string identifier);

	private:
		// List of current keybinds
		// sf::Keyboard::Key = keycode, KeyBind = properties for this bind.
		std::unordered_map<std::string, KeyBind> _keyBinds;
		std::unordered_map<std::string, MouseBind> _mouseBinds;
		
		sf::Event *_event;
		
	};

}