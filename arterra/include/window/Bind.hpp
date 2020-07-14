#pragma once

#include "PCH.hpp"

#include <chrono>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "util/DataObject.hpp"

namespace arterra {
	
	struct KeyBindData {
		// Whether they key is currently actively pressed or not.
		bool _isActive;
		// The time the key has been currently pressed down for.
		float_t _timePressed;
		
		KeyBindData();
		KeyBindData(bool isActive, float_t timePressed);
		
	};
	
	class KeyBind : public DataObject {
		
		public:
			KeyBind(sf::Keyboard::Key key);
			// Keeps track of whether they key changes state.
			void Update();
			// Generate data struct for this keybind.
			KeyBindData GetData();
			
			virtual void DumpToLog(std::string title = "--- KEYBIND ---") override;
		
		private:
			// The keycode for this keybind.
			sf::Keyboard::Key _keyCode;
			// Whether the key is currently actively pressed or not.
			bool _isActive;
			// Time when the key was initially pressed.
			std::chrono::high_resolution_clock::time_point _pressedTime;
			
	};
	
}