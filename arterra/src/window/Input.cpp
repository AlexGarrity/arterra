#include "window/Input.hpp"

namespace arterra {


	Input::Input(sf::Event &event) {
		_event = &event;
	}

	void Input::Update(float_t deltaTime)
	{
		// Iterate through all keys to check if they're getting pressed down,
		// to start the held timer.
		for (auto& it: _keyBinds) {
			it.second.Update();
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


}