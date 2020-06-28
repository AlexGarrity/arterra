#include <iostream>

#include <gl/gl3w.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

int main()
{
    glm::vec3 {1, 2, 3};
    gl3wInit();
    glfwInit();
    std::cout << "Hello world" << std::endl;
    return 0;
}