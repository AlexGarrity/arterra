#pragma once

#include "PCH.hpp"

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
			// Time when the key was initially pressed/released.
			std::chrono::high_resolution_clock::time_point _pressedTime;
			
	};
	
	struct MouseBindData {
		// Whether they button is currently actively pressed or not.
		bool _isActive;
		// The time the button has been currently pressed down for.
		float_t _timePressed;
		
		MouseBindData();
		MouseBindData(bool isActive, float_t timePressed);
	};
	
	class MouseBind : public DataObject {
		
		public:
			MouseBind(sf::Mouse::Button button);
			// Keeps track of whether they button changes state.
			void Update();
			// Generate data struct for this mousebind.
			MouseBindData GetData();
			
			virtual void DumpToLog(std::string title = "--- MOUSEBIND ---") override;
		
		private:
			// The buttonCode for this mousebind.
			sf::Mouse::Button _buttonCode;
			// Whether the button is currently actively pressed or not.
			bool _isActive;
			// Time when the button was pressed/released.
			std::chrono::high_resolution_clock::time_point _pressedTime;
		
	};
	
	struct MouseAxisData {
		// The movement amount on this axis.
		float_t _delta;
		
		MouseAxisData();
		MouseAxisData(float_t delta);
	};

	
}