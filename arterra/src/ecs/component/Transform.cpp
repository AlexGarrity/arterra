#include "ecs/component/Transform.hpp"

namespace arterra {

    Transform::Transform(glm::vec3 position, glm::quat rotation, Transform *parent)
    : _position { position }
			, _rotation { rotation }
			, _parent { parent }
            		{
			
		}

    void Transform::Update()
    {

    }

    glm::vec3 Transform::Position() const {
        if (_parent)
            return _position + _parent->_position;
        return _position;
    }

    glm::quat Transform::Rotation() const {
        if (_parent)
            return _rotation + _parent->_rotation;
        return _rotation;
    }

    
    glm::vec3 &Transform::Position() {
        return _position;
    }

    glm::quat &Transform::Rotation() {
        return _rotation;
    }


    glm::vec3 Transform::Up() const {
        return { 0.0f, 1.0f, 0.0f };
    }

    glm::vec3 Transform::Forward() const {
        return { 0.0f, 0.0f, -1.0f };
    }

    glm::vec3 Transform::Right() const {
        return glm::normalize(glm::cross(Forward(), Up()));
    }


    void Transform::Translate(glm::vec3 translation) {
        _position += translation;
    }

    void Transform::Rotate(glm::quat rotation) {
        _rotation += rotation;
    }

}