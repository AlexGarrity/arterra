#include "ecs/component/GameObject.hpp"

namespace arterra {

    void GameObject::Update() {
        _transform.Update();
    }


}