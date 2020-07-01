#pragma once

#include "PCH.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ecs/component/Base.hpp"

namespace arterra {

	// Basic transform component
	struct Transform : public ComponentBase {

		// Position as a Vector (x,y,z).
		glm::vec3 _position;
		// Vectors for the 3 axial directions.
		glm::vec3 _up { 0.0f, 1.0f, 0.0f };
		glm::vec3 _forward { 0.0f, 0.0f, -1.0f };
		glm::vec3 _right { 0.0f };
		// Rotation as a Quaternion (x,y,z,w).
		glm::quat _rotation;
		// Parent object transform (if any).
		Transform* _parent;

		virtual void Update() override
		{
			// Update position to account for parent movement (if any).
			if (_parent) {
				// TODO: Fix infinite movement
				_position += _parent->_position;
				_rotation += _parent->_rotation;
			}
			// Recalculate the right vector in case forward/up changes.
			_right = glm::normalize(glm::cross(_forward, _up));
		}

		// Full ctor.  Parent should be world if nothing else.
		Transform(glm::vec3 position = glm::vec3(), glm::quat rotation = glm::quat(), Transform* parent = nullptr)
			: _position { position }
			, _rotation { rotation }
			, _parent { parent }
		{
			Update();
		}

		// Basic ctor. Sets the position and rotation to that of the parent.
		Transform(Transform* parent)
			: _parent { parent }
		{
			Update();
		}
	};

} // namespace arterra