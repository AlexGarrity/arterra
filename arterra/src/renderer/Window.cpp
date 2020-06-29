#include "renderer/Window.hpp"

namespace arterra {

    Window::Window(uint32_t width, uint32_t height, std::string title) 
    : _width{width}, _height{height}, _title{title} {

        // Initialise GLFW
        if (!glfwInit()) {
            // If that fails, error and return
            Logger::Get().Log("GLFW failed to initialise", Logger::Severity::Fatal);
        }

        // Sets GL Version to 4.0
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        // Sets GL profile to core
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        // Create a unique ptr to the window
        _window = std::make_unique<GLFWwindow>(glfwCreateWindow(width, height, title.data(), nullptr, nullptr));
    }

}