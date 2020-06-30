#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window/Window.hpp"
#include "renderer/Renderer.hpp"

using namespace arterra;

int main()
{   
	Window window {1280, 720, "Arterra"};
	window.SetVsync(true);
	window.SetClearColour(0.0f, 1.0f, 1.0f, 1.0f);

	Renderer renderer;
	renderer.Init();
	
	// While window shouldn't close
	while (!window.ShouldClose()) {
		// Clear the window
		window.Clear();
		renderer.Update();
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
