#include "window/Input.hpp"

namespace arterra {

	Input::Input(sf::Event& event, Window* window)
	{
		_event = &event;
		_window = window;
	}

	void Input::Update(float_t deltaTime)
	{
		// Iterate through all keys to check if they're getting pressed down,
		// to start the held timer.
		for (auto& it : _keyBinds) {
			it.second.Update();
		}
		for (auto& it : _mouseBinds) {
			it.second.Update();
		}

		int halfWidth = _window->GetWidth() / 2;
		int halfHeight = _window->GetHeight() / 2;

		int windowHalfX = _window->GetPosition().x + halfWidth;
		int windowHalfY = _window->GetPosition().y + halfHeight;

		auto mouseX = sf::Mouse::getPosition().x;
		auto mouseY = sf::Mouse::getPosition().y;

		auto dx = ((mouseX - windowHalfX) / static_cast<float>(halfWidth)) * _inputSensitivity.x;
		auto dy = ((mouseY - windowHalfY) / static_cast<float>(halfHeight)) * _inputSensitivity.y;

		_mouseDelta = sf::Vector2f(-dx, -dy);

		Logger::Get().Log("dx: ", dx, "; dy: ", dy);

		if (_window->GetEvent().type == sf::Event::LostFocus)
			SetLockCursor(false);
		if (_window->GetEvent().type == sf::Event::GainedFocus)
			SetLockCursor(true);

		// Re-centre cursor if it's locked.
		if (_cursorLocked) {
			sf::Mouse::setPosition(sf::Vector2i(windowHalfX, windowHalfY));
		}
	}

	void Input::RegisterKeyBind(const std::string identifier, sf::Keyboard::Key key)
	{
		auto it = _keyBinds.find(identifier);
		if (it != _keyBinds.end()) {
			Logger::Get().Log(Logger::Warning, "Trying to bind key `", key, "` which already is bound!");
			return;
		}
		_keyBinds.emplace(identifier, key);
	}

	KeyBindData Input::PollKeyBind(const std::string identifier)
	{
		auto it = _keyBinds.find(identifier);
		if (it == _keyBinds.end()) {
			Logger::Get().Log(Logger::Warning, "Trying to access keybind `", identifier, "' which doesn't exist!");
			return KeyBindData();
		}
		return it->second.GetData();
	}

	void Input::RegisterMouseBind(const std::string identifier, sf::Mouse::Button button)
	{
		auto it = _mouseBinds.find(identifier);
		if (it != _mouseBinds.end()) {
			Logger::Get().Log(Logger::Warning, "Trying to bind button `", button, "` which already is bound!");
			return;
		}
		_mouseBinds.emplace(identifier, button);
	}

	void Input::SetMouseSensitivity(MouseAxis axis, float_t sensitivity) {
		if (axis == Horizontal) {
			_inputSensitivity.x = sensitivity;
		}
		else {
			_inputSensitivity.x = sensitivity;
		}
	}

	void Input::SetLockCursor(bool lock)
	{
		auto &window = _window->GetHandle();
		_cursorLocked = lock;
		(_cursorLocked) ? window.setMouseCursorVisible(false) : window.setMouseCursorVisible(true);
		(_cursorLocked) ? window.setMouseCursorGrabbed(true) : window.setMouseCursorGrabbed(false);
	}


	MouseBindData Input::PollMouseBind(const std::string identifier)
	{
		auto it = _mouseBinds.find(identifier);
		if (it == _mouseBinds.end()) {
			Logger::Get().Log(
				Logger::Warning, "Trying to access mousebind `", identifier, "' which doesn't exist!");
			return MouseBindData();
		}
		return it->second.GetData();
	}

	// ===Mouse Axis===
	MouseAxisData Input::PollMouseAxis(MouseAxis axis)
	{
		if (axis == MouseAxis::Horizontal) {
			return MouseAxisData { static_cast<float_t>(_mouseDelta.x) };
		} else if (axis == MouseAxis::Vertical) {
			return MouseAxisData { static_cast<float_t>(_mouseDelta.y) };
		}
		return MouseAxisData {};
	}

}