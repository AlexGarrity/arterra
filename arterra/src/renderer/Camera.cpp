#include "renderer/Camera.hpp"

#include "window/Window.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace arterra {

	Camera::Camera()
	{
		// Create a new perspective projection
		_projection = glm::perspective(glm::radians(55.0f), 16.f / 9.f, 0.1f, 1000.0f);
	}

	void Camera::Update(Window& window, float deltaTime)
	{
		// Update the ortho projection in case the window gets resized.
		_guiProjection = glm::ortho(
			0.0f, static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()), 0.0f);

		_transform.Update();

		// Update the view projection to account for movement
		_view = glm::mat4 { 1.0f };
		_viewProjection = glm::mat4 { 1.0f };
		_view = glm::lookAt(_transform.Position(), _transform.Position() + _transform.Forward(), _transform.Up());
		_viewProjection = _projection * _view;
		_viewFrustum.Update(_viewProjection);
	}

	glm::mat4 Camera::ViewProjection() const
	{
		return _viewProjection;
	}
	
	glm::mat4 Camera::GuiProjection() const
	{
		return _guiProjection;
	}

	ViewFrustum& Camera::GetViewFrustum()
	{
		return _viewFrustum;
	}
	
	Transform& Camera::GetTransform()
	{
		return _transform;
	}

}
