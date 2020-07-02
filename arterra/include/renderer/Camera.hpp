#pragma once

#include "PCH.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/Time.hpp"
#include "window/Window.hpp"

#include "ecs/component/Transform.hpp"

namespace arterra {

class Camera {
public:
    Camera()
    {
        // Create a new perspective projection
        _projection = glm::perspective(glm::radians(76.0f), 16.f / 9.f, 0.1f, 100.0f);

        // Init position and rotation
        _transform._position = glm::vec3 { 0.0f };
        _transform._rotation = glm::vec3 { 0.0f };

        // Set camera axis
        _transform._forward = glm::vec3(0.0f, 0.0f, -1.0f);
        _transform._up = glm::vec3(0.0f, 1.0f, 0.0f);
        _transform._right = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    void Update(Window& window)
    {

        // Set speed to account for deltaTime
        _speed = Time::GetDeltaTime() * 2.0f;
        _transform.Update();

        // Whole bunch of input handling
        if (glfwGetKey(window.GetHandle(), GLFW_KEY_W) == GLFW_PRESS) {
            _transform._position -= _transform._forward * _speed;
        }
        if (glfwGetKey(window.GetHandle(), GLFW_KEY_S) == GLFW_PRESS) {
            _transform._position += _transform._forward * _speed;
        }

        if (glfwGetKey(window.GetHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            _transform._position -= _transform._up * _speed;
        }
        if (glfwGetKey(window.GetHandle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            _transform._position += _transform._up * _speed;
        }

        if (glfwGetKey(window.GetHandle(), GLFW_KEY_A) == GLFW_PRESS) {
            _transform._position += _transform._right * _speed;
        }
        if (glfwGetKey(window.GetHandle(), GLFW_KEY_D) == GLFW_PRESS) {
            _transform._position -= _transform._right * _speed;
        }

        // Update the view projection to account for movement
        _view = glm::mat4 { 1.0f };
        _viewProjection = glm::mat4 { 1.0f };
        _view = glm::translate(_view, _transform._position);
        // Convert Quaternion to Euler angles
        auto euler = glm::eulerAngles(_transform._rotation);
        _view = glm::rotate(_view, glm::radians(euler.x), glm::vec3(1.0f, 0.0f, 0.0f));
        _view = glm::rotate(_view, glm::radians(euler.y), glm::vec3(0.0f, 1.0f, 0.0f));
        _view = glm::rotate(_view, glm::radians(euler.z), glm::vec3(0.0f, 0.0f, 1.0f));
        // Create the view projection
        _viewProjection = _projection * _view;
    }

    // Get the view projection
    inline glm::mat4 ViewProjection() const { return _viewProjection; }

private:
    float_t _speed = 2.0f;

    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _viewProjection;

    Transform _transform;
};

};