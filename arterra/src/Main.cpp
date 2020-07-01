#include "PCH.hpp"

#define STB_IMAGE_IMPLEMENTATION

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
	// Set working directory to executable directory
	std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
	Logger::Get().Log(Logger::Debug, "Executable directory: '", std::filesystem::current_path(), "'");
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

	float_t timeToResourceUnload = 5.0f;
	
	Time::CalculateDeltaTime();

	// While window shouldn't close
	while (!window.ShouldClose()) {
		Time::CalculateDeltaTime();

		// Clear the window
		window.Clear();
		camera.Update(window);
		renderer.Update();
		renderer.DrawTestCube();
		// Check for events and swap buffers
		window.Update();
		
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			window.SetShouldClose(true);
		}

		timeToResourceUnload -= Time::GetDeltaTime();
		if (timeToResourceUnload < 0.0f) {
			Resource::Get().Unload();
			timeToResourceUnload = 5.0f;
		}
	}

	// Exit successfully
	return 0;
}
