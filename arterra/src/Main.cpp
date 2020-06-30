#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window/Window.hpp"
#include "renderer/Renderer.hpp"

using namespace arterra;

int main(int argc, char **argv)
{   
	Logger::Get().Log(argv[0], Logger::Severity::Debug);
	Window window {1280, 720, "Arterra"};
	window.SetVsync(true);
	window.SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);

	Renderer renderer;
	
	// While window shouldn't close
	while (!window.ShouldClose()) {
		// Clear the window
		window.Clear();
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
