#pragma once

#include "PCH.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ecs/component/Base.hpp"

namespace arterra {

// Basic transform component
struct Transform : public ComponentBase {

    // Position as x,y,z
    glm::vec3 _position;
    // Directions
    glm::vec3 _up { 0.0f, 1.0f, 0.0f };
    glm::vec3 _forward { 0.0f, 0.0f, -1.0f };
    glm::vec3 _right { 0.0f };
    // Rotation as a Quaternion x,y,z,w
    glm::quat _rotation;
    // Parent object
    Transform *_parent;

    // Update position to account for parent movement
    void Update() override
    {
        if (_parent) {
            _position += _parent->_position;
            _rotation += _parent->_rotation;
        }
        _right = glm::normalize(glm::cross(_forward, _up));
    }

    // Full ctor.  Parent should be world if nothing else
    Transform(glm::vec3 position = glm::vec3(), glm::quat rotation = glm::quat(),
        Transform* parent = nullptr)
        : _position { position }
        , _rotation { rotation }
        , _parent { parent }
    {
        Update();
    }

    // Basic ctor sets position and rotation to that of the parent
    Transform(Transform* parent)
        : _parent { parent }
    {
        Update();
    }

};

} // namespace arterra