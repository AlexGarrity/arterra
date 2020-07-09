#pragma once

#include "PCH.hpp"

#include "ecs/component/Base.hpp"
#include "ecs/component/Transform.hpp"

namespace arterra {

	class GameObject : public ComponentBase {
	public:
		// Update function for all objects.
		void Update() override;

	private:
		// The transform for this object in the world.
		Transform _transform;
	};

}