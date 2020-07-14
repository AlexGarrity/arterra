#include "window/Input.hpp"

namespace arterra {


	Input::Input(sf::Event &event, Window *window) {
		_event = &event;
		_window = window;
	}

	void Input::Update(float_t deltaTime)
	{
		// Iterate through all keys to check if they're getting pressed down,
		// to start the held timer.
		for (auto& it: _keyBinds) {
			it.second.Update();
		}
		for (auto& it: _mouseBinds) {
			it.second.Update();
		}
		
		if (_event->type == sf::Event::MouseMoved) {
			// Calculate mouse movement since last frame.
			sf::Vector2i currentPos = sf::Vector2i { _event->mouseMove.x, _event->mouseMove.y };
			_mouseDelta = -(currentPos - sf::Vector2i(_window->GetWidth() / 2, _window->GetHeight() / 2));
			
		}
		
	}
	
	void Input::RegisterKeyBind(const std::string identifier, sf::Keyboard::Key key) {
		auto it = _keyBinds.find(identifier);
		if (it != _keyBinds.end()) {
			Logger::Get().Log(Logger::Warning, "Trying to bind key `", key, "` which already is bound!");
			return;
		}
		_keyBinds.emplace(identifier, key);
	}
	
	KeyBindData Input::PollKeyBind(const std::string identifier) {
		auto it = _keyBinds.find(identifier);
		if (it == _keyBinds.end()){
			Logger::Get().Log(Logger::Warning, "Trying to access keybind `", identifier, "' which doesn't exist!");
			return KeyBindData();
		}
		return it->second.GetData();
	}

	void Input::RegisterMouseBind(const std::string identifier, sf::Mouse::Button button) {
		auto it = _mouseBinds.find(identifier);
		if (it != _mouseBinds.end()) {
			Logger::Get().Log(Logger::Warning, "Trying to bind button `", button, "` which already is bound!");
			return;
		}
		_mouseBinds.emplace(identifier, button);
	}
	
	MouseBindData Input::PollMouseBind(const std::string identifier) {
		auto it = _mouseBinds.find(identifier);
		if (it == _mouseBinds.end()){
			Logger::Get().Log(Logger::Warning, "Trying to access mousebind `", identifier, "' which doesn't exist!");
			return MouseBindData();
		}
		return it->second.GetData();
	}
	
	// ===Mouse Axis===
	MouseAxisData Input::PollMouseAxis(MouseAxis axis) {
		if (axis == MouseAxis::Horizontal) {
			return MouseAxisData { static_cast<float_t>(_mouseDelta.x) };
		}else if (axis == MouseAxis::Vertical) {
			return MouseAxisData { static_cast<float_t>(_mouseDelta.y)  };
		}
		return MouseAxisData {};
	}

}