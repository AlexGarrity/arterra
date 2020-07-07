#pragma once

#include "PCH.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ecs/component/Base.hpp"

namespace arterra {

	// Basic transform component
	class Transform : public ComponentBase {
	public:
		virtual void Update() override;

		// Full ctor.  Parent should be world if nothing else.
		Transform(glm::vec3 position = glm::vec3(), glm::quat rotation = glm::quat(), Transform* parent = nullptr);

		// Position getters
		glm::vec3 Position() const;
		glm::quat Rotation() const;

		// Raw position getter-setter
		glm::vec3& Position();
		glm::quat& Rotation();

		// Direction getters
		glm::vec3 Up() const;
		glm::vec3 Forward() const;
		glm::vec3 Right() const;

		void Translate(glm::vec3 translation);
		void Rotate(glm::quat rotation);

	private:
		// Position as a Vector (x,y,z).
		glm::vec3 _position;
		// Rotation as a Quaternion (x,y,z,w).
		glm::quat _rotation;
		// Parent object transform (if any).
		Transform* _parent;
	};

} // namespace arterra