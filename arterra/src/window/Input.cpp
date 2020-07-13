#include "window/Input.hpp"

namespace arterra {

	KeyBind::KeyBind() {}

	void KeyBind::Update(float deltaTime) {
		auto pressed = sf::Keyboard::isKeyPressed(_keyCode);
		if (_active && !pressed) {
			_active = false;
			_timeHeld = 0.0f;
		}
		else {
			_active = true;
			_timeHeld += deltaTime;
		}
	}

	void KeyBind::CallSubscribers()
	{
		for (auto i : _subscribers) {
			i(_timeHeld);
		}
	}

	void KeyBind::AddSubscriber(std::function<void(float_t)> subscriber) {
		_subscribers.push_back(subscriber);
	}

	void KeyBind::DumpToLog(std::string title)
	{
		Logger::Get().Log("\t", title, " - code: ", _keyCode, "; active:", _active, "; time held:", _timeHeld,
			"; sub count: ", _subscribers.size());
	}

	Input::Input(sf::Event &event) {
		_event = &event;
	}

	void Input::Update(float_t deltaTime)
	{
		// If no keys are being tracked, return quickly.
		if (_pressedKeys.empty()) {
			return;
		}

		// Increase the hold time for all actively held keys.
		for (auto& i : _pressedKeys) {
			i.second.Update(deltaTime);
		}

		// DEBUG
		// PrintInformation();
	}

	void Input::RegisterKeySubscriber(const sf::Keyboard::Key key, const std::function<void(float_t)> callback)
	{
		auto keyIndex = _pressedKeys.find(key);
		if (keyIndex == _pressedKeys.end())
			return;
		// Add the callback function to the key subscription.
		keyIndex->second.AddSubscriber(callback);
	}

	void Input::PrintInformation()
	{
		// Log the pressed keys information.
		Logger::Get().Log(Logger::Debug, "===================");
		Logger::Get().Log(Logger::Debug, " KEY INPUTS STORED ");
		for (auto& i : _pressedKeys) {
			i.second.DumpToLog();
		}
		Logger::Get().Log(Logger::Debug, "===================");
	}

}