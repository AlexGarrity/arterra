#include "PCH.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "window/Input.hpp"
#include "window/Window.hpp"

using namespace arterra;

int main(int argc, char** argv)
{
	// Set working directory to executable directory
	std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
	Logger::Get().Log(Logger::Debug, "Executable directory: '", std::filesystem::current_path(), "'");

	// Create window
	Window window { 1280, 720, "Arterra" };
	window.SetVsync(true);
	window.SetClearColour(0.0f, 1.0f, 1.0f, 1.0f);

	// Register input callbacks
	glfwSetKeyCallback(window.GetHandle(), &Input::KeyCallback);

	// Load GL core using GLAD, if it fails then error and return
	if (!gladLoadGL()) {
		std::cerr << "GLAD failed to initialise" << std::endl;
		return -1;
	}

	// Give GLAD the GLFW extension loader function
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	// Initialise a camera and renderer
	Camera camera;
	Renderer renderer { camera };

	// Set some values before running the first game loop
	float_t timeToResourceUnload = 5.0f;
	Time::CalculateDeltaTime();

	// While core game loop is running
	while (!window.ShouldClose()) {
		Time::CalculateDeltaTime();

		// Clear the window
		window.Clear();
		camera.Update(window);
		renderer.Update();
		renderer.DrawTestCube();
		// Poll events and swap buffers
		window.Update();

		// Close window with [Esc]
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			window.SetShouldClose(true);
		}

		// Every 5 seconds, perform garbage collection
		timeToResourceUnload -= Time::GetDeltaTime();
		if (timeToResourceUnload < 0.0f) {
			Resource::Get().Unload();
			timeToResourceUnload = 5.0f;
		}
	}

	// Exit successfully
	return 0;
}
