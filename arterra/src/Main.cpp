#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window/Window.hpp"
#include "window/Input.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Camera.hpp"

using namespace arterra;

int main(int argc, char **argv)
{   
	Logger::Get().Log(Logger::Debug, "Executable directory: '", argv[0], "'");
	Window window {1280, 720, "Arterra"};
	window.SetVsync(true);
	window.SetClearColour(0.0f, 1.0f, 1.0f, 1.0f);
	
	glfwSetKeyCallback(window.GetHandle(), &Input::KeyCallback);
	
	// Load GL core using GLAD, if it fails then error and return
	if (!gladLoadGL()) {
		std::cerr << "GLAD failed to initialise" << std::endl;
		return -1;
	}

	// Give GLAD the GLFW extension loader function
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	Camera camera;
	Renderer renderer {camera};
	
	// While window shouldn't close
	while (!window.ShouldClose()) {
		// Clear the window
		window.Clear();
		camera.Update(window);
		renderer.Update();
		renderer.DrawTestCube();
		// Check for events and swap buffers
		window.Update();

		Time::CalculateDeltaTime();
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			window.SetShouldClose(true);
		}
	}

	// Exit successfully
	return 0;
}
