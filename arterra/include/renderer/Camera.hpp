#pragma once

#include "PCH.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/Time.hpp"
#include "window/Window.hpp"

namespace arterra {

    class Camera {
        public:
            Camera() {
                // Create a new perspective projection
                _projection = glm::perspective(glm::radians(76.0f), 16.f/9.f, 0.1f, 100.0f);

                // Init position and rotation
                _position = glm::vec3 { 0.0f };
                _rotation = glm::vec3 { 0.0f };

                // Set camera axis
                _forward = glm::vec3(0.0f, 0.0f, -1.0f);
                _up = glm::vec3(0.0f, 1.0f, 0.0f);
                _right = glm::vec3(1.0f, 0.0f, 0.0f);
            }

            void Update(Window &window) {

                // Set speed to account for deltaTime
                _speed = Time::GetDeltaTime() * 2.0f;
                // Update right so rotation works properly
                _right = -glm::normalize(glm::cross(_up, _forward));

                // Whole bunch of input handling
                if (glfwGetKey(window.GetHandle(), GLFW_KEY_W) == GLFW_PRESS) {
                    _position -= _forward * _speed;
                }
                if (glfwGetKey(window.GetHandle(), GLFW_KEY_S) == GLFW_PRESS) {
                   _position += _forward * _speed;
                }

                if (glfwGetKey(window.GetHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                    _position -= _up * _speed;
                }
                if (glfwGetKey(window.GetHandle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                    _position += _up * _speed;
                }

                if (glfwGetKey(window.GetHandle(), GLFW_KEY_A) == GLFW_PRESS) {
                    _position += _right * _speed;
                }
                if (glfwGetKey(window.GetHandle(), GLFW_KEY_D) == GLFW_PRESS) {
                    _position -= _right * _speed;
                }

                // Update the view projection to account for movement
                _view = glm::mat4 { 1.0f };
                _viewProjection = glm::mat4 { 1.0f };
                _view = glm::translate(_view, _position);
                _view = glm::rotate(_view, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                _view = glm::rotate(_view, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                _view = glm::rotate(_view, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                _viewProjection = _projection * _view;
            }

            // Get the view projection
            inline glm::mat4 ViewProjection() const { return _viewProjection; }

        private:
            float_t _speed = 2.0f;

            glm::mat4 _projection;
            glm::mat4 _view;
            glm::mat4 _viewProjection;

            glm::vec3 _position;
            glm::vec3 _rotation;
            glm::vec3 _forward;
            glm::vec3 _up;
            glm::vec3 _right;

    };

};