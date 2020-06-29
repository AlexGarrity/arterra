#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "renderer/Window.hpp"

using namespace arterra;

int main()
{   
	Window window {1280, 720, "Arterra"};
	window.SetVsync(true);
	window.SetClearColour(0.0f, 1.0f, 1.0f, 1.0f);
	
	// Load GL core using GLAD, if it fails then error and return
	if (!gladLoadGL()) {
		std::cerr << "GLAD failed to initialise" << std::endl;
		return -1;
	}

	// Give GLAD the GLFW extension loader function
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	// While window shouldn't close
	while (!window.ShouldClose()) {
		// Clear the window
		window.Clear();
		// Check for events and swap buffers
		window.Update();

		Time::CalculateDeltaTime();
		Logger::Get().Log(std::to_string(Time::GetDeltaTime()).data(), Logger::Severity::Debug);

		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			window.SetShouldClose(true);
		}
	}

	// Exit successfully
	return 0;
}
