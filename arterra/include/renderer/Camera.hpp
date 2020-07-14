#pragma once

#include "PCH.hpp"

#include "window/Window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ecs/component/Transform.hpp"
#include "world/Block.hpp"

namespace arterra {

	class ViewFrustum {
		struct Plane {
			glm::vec3 normal;
			float d;

			float DistanceToPoint(glm::vec3 point);
		};

	public:
		ViewFrustum();
		void Update(glm::mat4& viewProjetionMatrix);

		bool PointInFrustum(glm::vec3 point);
		bool ChunkInFrustum(WorldPosition pos);

	private:
		std::array<Plane, 6> _planes;
	};

	class Camera {
	public:
		Camera();

		// Update camera position and view projection matrix
		void Update(Window& window, float_t deltaTime);

		// Get the view projection
		inline glm::mat4 ViewProjection() const { return _viewProjection; }
		inline ViewFrustum& GetViewFrustum() { return _viewFrustum; }
		
		inline Transform& GetTransform() { return _transform; }

	private:
		glm::mat4 _projection;
		glm::mat4 _view;
		glm::mat4 _viewProjection;

		glm::vec3 _viewDirection;

		Transform _transform;
		ViewFrustum _viewFrustum;
	};

};