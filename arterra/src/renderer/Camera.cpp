#include "renderer/Camera.hpp"

namespace arterra {

	Camera::Camera()
	{
		// Create a new perspective projection
		_projection = glm::perspective(glm::radians(76.0f), 16.f / 9.f, 0.1f, 1000.0f);
	}

	void Camera::Update(Window& window, float_t deltaTime)
	{
		// Set speed to account for deltaTime
		_speed = deltaTime * 15.0f;
		_transform.Update();

		// Whole bunch of input handling
		if (glfwGetKey(window.GetHandle(), GLFW_KEY_W) == GLFW_PRESS) {
			_transform.Position() -= _transform.Forward() * _speed;
		}
		if (glfwGetKey(window.GetHandle(), GLFW_KEY_S) == GLFW_PRESS) {
			_transform.Position() += _transform.Forward() * _speed;
		}

		if (glfwGetKey(window.GetHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			_transform.Position() -= _transform.Up() * _speed;
		}
		if (glfwGetKey(window.GetHandle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			_transform.Position() += _transform.Up() * _speed;
		}

		if (glfwGetKey(window.GetHandle(), GLFW_KEY_A) == GLFW_PRESS) {
			_transform.Position() += _transform.Right() * _speed;
		}
		if (glfwGetKey(window.GetHandle(), GLFW_KEY_D) == GLFW_PRESS) {
			_transform.Position() -= _transform.Right() * _speed;
		}

		// Update the view projection to account for movement
		_view = glm::mat4 { 1.0f };
		_viewProjection = glm::mat4 { 1.0f };
		_view = glm::translate(_view, _transform.Position());
		// Convert Quaternion to Euler angles
		/*
		auto euler = glm::eulerAngles(_transform.Rotation());
		_view = glm::rotate(_view, glm::radians(euler.x), glm::vec3(1.0f, 0.0f, 0.0f));
		_view = glm::rotate(_view, glm::radians(euler.y), glm::vec3(0.0f, 1.0f, 0.0f));
		_view = glm::rotate(_view, glm::radians(euler.z), glm::vec3(0.0f, 0.0f, 1.0f));
		*/
		// Create the view projection
		_viewProjection = _projection * _view;
	}

}