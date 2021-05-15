#include "window/Bind.hpp"

namespace arterra {
	
	// === Key Buttons===
	KeyBindData::KeyBindData()
		: _isActive(false), _timePressed(0.0f) {}
	
	KeyBindData::KeyBindData(bool isActive, float timePressed)
		: _isActive(isActive), _timePressed(timePressed) {}
	
	KeyBind::KeyBind(sf::Keyboard::Key key)
		: _keyCode(key), _isActive(false), _pressedTime(std::chrono::high_resolution_clock::now()) {}
	
	void KeyBind::Update() {
		
		if (sf::Keyboard::isKeyPressed(_keyCode)) {
			// If the keybind was previously inactive, but now is,
			// set the boolean and update the startTime.
			if (_isActive == false) {
				_isActive = true;
				_pressedTime = std::chrono::high_resolution_clock::now();
			}
		}else {
			// If the keybind was previously active, but now isn't,
			// set the boolean.
			if (_isActive == true) {
				_isActive = false;
				_pressedTime = std::chrono::high_resolution_clock::now();
			}
		}
		
	}
	
	KeyBindData KeyBind::GetData() const {
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		float duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _pressedTime).count() / 1000.0f;
		return KeyBindData { _isActive, duration };
	}
	
	void KeyBind::DumpToLog(const std::string &title) const {
		Logger::Debug( title);
		KeyBindData data = GetData(); 
		Logger::Debug( "KeyCode: ", _keyCode, " IsActive: ", data._isActive,
			" TimePressed: ", data._timePressed);
		Logger::Debug( "---------------");
	}
	
	// ===Mouse Buttons===
	MouseBindData::MouseBindData()
		: _isActive(false), _timePressed(0.0f) {}
	
	MouseBindData::MouseBindData(bool isActive, float timePressed)
		: _isActive(isActive), _timePressed(timePressed) {}
	
	MouseBind::MouseBind(sf::Mouse::Button button)
		: _buttonCode(button), _isActive(false), _pressedTime(std::chrono::high_resolution_clock::now()) {}
	
	void MouseBind::Update() {
		
		if (sf::Mouse::isButtonPressed(_buttonCode)) {
			// If the keybind was previously inactive, but now is,
			// set the boolean and update the startTime.
			if (_isActive == false) {
				_isActive = true;
				_pressedTime = std::chrono::high_resolution_clock::now();
			}
		}else {
			// If the keybind was previously active, but now isn't,
			// set the boolean.
			if (_isActive == true) {
				_isActive = false;
				_pressedTime = std::chrono::high_resolution_clock::now();
			}
		}
		
	}
	
	MouseBindData MouseBind::GetData() const {
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		float duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _pressedTime).count() / 1000.0f;
		return MouseBindData { _isActive, duration };
	}
	
	void MouseBind::DumpToLog(const std::string &title) const {
		Logger::Debug(title);
		MouseBindData data = GetData(); 
		Logger::Debug( "KeyCode: ", _buttonCode, " IsActive: ", data._isActive,
			" TimePressed: ", data._timePressed);
		Logger::Debug( "-----------------");
	}
	
	// ===Mouse Axis===
	MouseAxisData::MouseAxisData()
		: _delta(0.0f) {}
		
	MouseAxisData::MouseAxisData(float delta)
		: _delta(delta) {}
		
	
	
	
}