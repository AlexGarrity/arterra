#pragma once

#include "PCH.hpp"

#include "ecs/component/Base.hpp"
#include "ecs/component/Transform.hpp"

namespace arterra {

	struct GameObject : public ComponentBase {
		// The transform for this object in the world.
		Transform _transform;

		// Update function for all objects.
		void Update() override { _transform.Update(); }
	};

}