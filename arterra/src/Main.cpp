#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main()
{
    glm::vec3 vector {1.0f, 2.0f, 3.0f};
    auto x = vector.length();

    gladLoadGL();
    glfwInit();

    std::cout << "Hello world" << std::endl;
    return 0;
}