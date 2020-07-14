#include "window/Window.hpp"
#include "window/Input.hpp"

namespace arterra {

	Window::Window(int width, int height, std::string title)
		: _width { width }
		, _height { height }
		, _title { title }
	{

		sf::ContextSettings settings;
		settings.antialiasingLevel = 0;
		settings.depthBits = 24;
		settings.majorVersion = 4;
		settings.minorVersion = 0;
		settings.sRgbCapable = false;
		settings.stencilBits = 8;
		settings.attributeFlags = sf::ContextSettings::Attribute::Core;

		_window.create(sf::VideoMode(width, height), title, sf::Style::Default, settings);

		SetShouldClose(false);

		// Make this window the active GL context
		_window.setActive(true);

		// Loading GL straight after the context avoids access violation errors
		// Load GL core using GLAD, if it fails then error and return
		if (!gladLoadGL()) {
			Logger::Get().Log(Logger::Fatal, "GLAD failed to initialise");
		} else
			Logger::Get().Log(Logger::Debug, "Successfully initialised GLAD");
	}

	Window::~Window() { _window.close(); }

	// Poll for events
	void Window::PollEvents() { _window.pollEvent(_event); }

	// Swap the current buffer
	void Window::SwapBuffers() { _window.display(); }

	// Set colour to specified 0-1 floats
	void Window::SetClearColour(float_t red, float_t green, float_t blue, float_t alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	// Cast 0-255 values to 0-1 float_t, then set clear colour
	void Window::SetClearColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		// Multiply all values by inverse of 255
		float_t x = 1.0f / 254.99f;
		auto fRed = static_cast<float_t>(red) * x;
		auto fGreen = static_cast<float_t>(green) * x;
		auto fBlue = static_cast<float_t>(blue) * x;
		auto fAlpha = static_cast<float_t>(alpha) * x;
		// Call previous function to save retyping
		SetClearColour(fRed, fGreen, fBlue, fAlpha);
	}

	// Clear the window
	void Window::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	// Turn vsync on / off
	void Window::SetVsync(bool vsync)
	{
		_vsyncEnabled = vsync;
		// Ternary - if vsync == true then set vsync on, otherwise off
		_window.setVerticalSyncEnabled(vsync);
	}

	void Window::SetLockCursor(bool lock)
	{
		_cursorLocked = lock;
		(_cursorLocked) ? _window.setMouseCursorVisible(false) : _window.setMouseCursorVisible(true);
		(_cursorLocked) ? _window.setMouseCursorGrabbed(true) : _window.setMouseCursorGrabbed(false);
	}

	void Window::SetShouldClose(bool shouldClose) { _shouldClose = shouldClose; }

	void Window::Update(float_t deltaTime)
	{
		PollEvents();
		SwapBuffers();
		if (_event.type == sf::Event::Closed)
			SetShouldClose(true);
		auto size = _window.getSize();
		_width = size.x;
		_height = size.y;

		// Re-centre cursor if it's locked.
		if (_cursorLocked) {
			sf::Mouse::setPosition(sf::Vector2i(_width / 2, _height / 2), _window);
		}
		
		
	}
}