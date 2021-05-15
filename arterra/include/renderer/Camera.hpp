#pragma once

#include "PCH.hpp"

#include "renderer/ViewFrustum.hpp"
#include "ecs/component/Transform.hpp"

namespace arterra {

	class Window;
	class WorldPosition;
	class ViewFrustum;

	class Camera {
	public:
		Camera();

		// Update camera position and view projection matrix
		void Update(Window& window, float deltaTime);

		// Get the view projection
		glm::mat4 ViewProjection() const;
		glm::mat4 GuiProjection() const;
		ViewFrustum& GetViewFrustum();

		Transform& GetTransform();

	private:
		glm::mat4 _projection;
		glm::mat4 _view;
		glm::mat4 _viewProjection;

		glm::vec3 _viewDirection;

		Transform _transform;
		ViewFrustum _viewFrustum;

		glm::mat4 _guiProjection;
	};

};