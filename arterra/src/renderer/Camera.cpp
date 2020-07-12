#include "renderer/Camera.hpp"

namespace arterra {

	ViewFrustum::ViewFrustum() {}

	// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
	void ViewFrustum::Update(glm::mat4& vpMat)
	{
		// Left clipping plane
		_planes[0].b = vpMat[3][1] + vpMat[0][1];
		_planes[0].a = vpMat[3][0] + vpMat[0][0];
		_planes[0].c = vpMat[3][2] + vpMat[0][2];
		_planes[0].d = vpMat[3][3] + vpMat[0][3];
		// Right clipping plane
		_planes[1].a = vpMat[3][0] - vpMat[0][0];
		_planes[1].b = vpMat[3][1] - vpMat[0][1];
		_planes[1].c = vpMat[3][2] - vpMat[0][2];
		_planes[1].d = vpMat[3][3] - vpMat[0][3];
		// Top clipping plane
		_planes[2].a = vpMat[3][0] - vpMat[1][0];
		_planes[2].b = vpMat[3][1] - vpMat[1][1];
		_planes[2].c = vpMat[3][2] - vpMat[1][2];
		_planes[2].d = vpMat[3][3] - vpMat[1][3];
		// Bottom clipping plane
		_planes[3].a = vpMat[3][0] + vpMat[1][0];
		_planes[3].b = vpMat[3][1] + vpMat[1][1];
		_planes[3].c = vpMat[3][2] + vpMat[1][2];
		_planes[3].d = vpMat[3][3] + vpMat[1][3];
		// Near clipping plane
		_planes[4].a = vpMat[3][0] + vpMat[2][0];
		_planes[4].b = vpMat[3][1] + vpMat[2][1];
		_planes[4].c = vpMat[3][2] + vpMat[2][2];
		_planes[4].d = vpMat[3][3] + vpMat[2][3];
		// Far clipping plane
		_planes[5].a = vpMat[3][0] - vpMat[2][0];
		_planes[5].b = vpMat[3][1] - vpMat[2][1];
		_planes[5].c = vpMat[3][2] - vpMat[2][2];
		_planes[5].d = vpMat[3][3] - vpMat[2][3];

		for (auto& p : _planes) {
			float l = 1.0f / sqrtf(p.a * p.a + p.b * p.b + p.c * p.c);
			p.a *= l;
			p.b *= l;
			p.c *= l;
			p.d *= l;
		}
	}

	float ViewFrustum::Plane::DistanceToPoint(glm::vec3 point)
	{
		return a * point.x + b * point.y + c * point.z + d;
	}

	bool ViewFrustum::PointInFrustum(glm::vec3 point)
	{
		for (auto& p : _planes) {
			if (p.DistanceToPoint(point) < 0.0f)
				return false;
		}
		return true;
	}

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
		_viewFrustum.Update(_viewProjection);
	}

}