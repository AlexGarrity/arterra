#include "window/Window.hpp"
#include "window/Input.hpp"

namespace arterra {

	Window::Window(uint32_t width, uint32_t height, std::string title)
		: _width { width }
		, _height { height }
		, _title { title }
	{

		// Initialise GLFW
		if (!glfwInit()) {
			// If that fails, error and return
			Logger::Get().Log(Logger::Fatal, "GLFW failed to initialise");
		}

		// Sets GL Version to 4.0
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		// Sets GL profile to core
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		// Create a unique ptr to the window
		auto x = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(
			glfwCreateWindow(width, height, title.data(), nullptr, nullptr));
		_window.swap(x);

		SetShouldClose(false);

		// Make this window the active GL context
		glfwMakeContextCurrent(_window.get());

		// Loading GL straight after the context avoids access violation errors
		// Load GL core using GLAD, if it fails then error and return
		if (!gladLoadGL()) {
			Logger::Get().Log(Logger::Fatal, "GLAD failed to initialise");
		}
		else
			Logger::Get().Log(Logger::Debug, "Successfully initialised GLAD");
	}

	Window::~Window()
	{
		_window.reset();
		glfwTerminate();
	}

	// Poll for events
	void Window::PollEvents() { glfwPollEvents(); }

	// Swap the current buffer
	void Window::SwapBuffers() { glfwSwapBuffers(_window.get()); }

	// Set colour to specified 0-1 floats
	void Window::SetClearColour(float_t red, float_t green, float_t blue, float_t alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	// Cast 0-255 values to 0-1 float, then set clear colour
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
		(vsync) ? glfwSwapInterval(1) : glfwSwapInterval(0);
	}

	void Window::SetShouldClose(bool shouldClose)
	{
		_shouldClose = shouldClose;
		glfwSetWindowShouldClose(_window.get(), shouldClose);
	}

	void Window::Update(float_t deltaTime)
	{
		PollEvents();
		SwapBuffers();
	}
}