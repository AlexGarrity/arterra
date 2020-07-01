#pragma once

#include "PCH.hpp"

#include "ecs/component/Base.hpp"
#include "ecs/component/Transform.hpp"

namespace arterra {

    struct GameObject : public ComponentBase {
        Transform _transform;

        void Update() override {
            _transform.Update();
        }
    };

}