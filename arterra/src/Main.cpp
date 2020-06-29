#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main()
{   
	// Initialise GLFW
	if (!glfwInit()) {
		// If that fails, error and return
		std::cerr << "GLFW failed to initialise" << std::endl;
		return -1;
	}

	// Sets GL Version to 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// Sets GL profile to core
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a 1280x720 window called "Window"
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
	// Make the window the current OpenGL context
	glfwMakeContextCurrent(window);

	// If window failed to load, error and terminate GLFW
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// Load GL core using GLAD, if it fails then error and return
	if (!gladLoadGL()) {
		std::cerr << "GLAD failed to initialise" << std::endl;
		return -1;
	}

	// Give GLAD the GLFW extension loader function
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	// While window shouldn't close
	while (!glfwWindowShouldClose(window)) {
		// Set the clear colour to magenta
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		// Clear the screen to the colour of the GL Buffer bit (black)
		glClear(GL_COLOR_BUFFER_BIT);
		// Check for events in GLFW
		glfwPollEvents();
		// Swap the window buffers (new frame)
		glfwSwapBuffers(window);
		// If escape is pressed, exit
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}


	// Exit successfully
	return 0;
}
