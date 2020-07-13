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
		bool ChunkInFrustum(BlockPosition pos);

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

	private:
		float_t _speed = 2.0f;
		float_t _rotSpeed = glm::radians(15.0f);

		glm::mat4 _projection;
		glm::mat4 _view;
		glm::mat4 _viewProjection;

		glm::vec3 _viewDirection;

		Transform _transform;
		ViewFrustum _viewFrustum;
	};

};