#pragma once

#include "PCH.hpp"

#include <glm/gtc/quaternion.hpp>

#include "ecs/component/Base.hpp"

namespace arterra {

	// Basic transform component
	class Transform : public ComponentBase {
	public:
		virtual void Update() override;

		// Full ctor.  Parent should be world if nothing else.
		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f),
			Transform* parent = nullptr);

		// Position getters
		glm::vec3 Position() const;
		glm::vec3 Rotation() const;

		// Raw position getter-setter
		glm::vec3& Position();
		glm::vec3& Rotation();

		// Direction getters
		glm::vec3 Up() const;
		glm::vec3 Forward() const;
		glm::vec3 Right() const;

		void Translate(glm::vec3 translation);
		void Rotate(glm::quat rotation);
		void Rotate(glm::vec3 rotation);
		void Rotate(float_t x, float_t y, float_t z);

	private:
		// Position as a Vector (x,y,z).
		glm::vec3 _position;
		// Direction vectors
		glm::vec3 _forward, _right, _up;
		// Rotation as a Quaternion (x,y,z,w).
		glm::vec3 _rotation;
		// Parent object transform (if any).
		Transform* _parent;
	};

} // namespace arterra