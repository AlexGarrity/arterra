#include "ecs/component/Transform.hpp"

namespace arterra {

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, Transform* parent)
		: _position { position }
		, _rotation { rotation }
		, _parent { parent }
	{
	}

	void Transform::Update()
	{
		_forward = glm::normalize(glm::vec3(
			cosf(_rotation.y) * cosf(_rotation.x), sinf(_rotation.x), sinf(_rotation.y) * cosf(_rotation.x)));
		_right = glm::normalize(glm::cross(_forward, glm::vec3{0.0f, 1.0f, 0.0f}));
		_up = glm::normalize(glm::cross(_right, _forward));
}

	glm::vec3 Transform::Position() const
	{
		if (_parent)
			return _position + _parent->_position;
		return _position;
	}

	glm::vec3 Transform::Rotation() const
	{
		if (_parent)
			return _rotation + _parent->_rotation;
		return _rotation;
	}

	glm::vec3& Transform::Position() { return _position; }

	glm::vec3& Transform::Rotation() { return _rotation; }

	glm::vec3 Transform::Up() const { return _up; }

	glm::vec3 Transform::Forward() const { return _forward; }

	glm::vec3 Transform::Right() const { return _right; }

	void Transform::Translate(glm::vec3 translation) { _position += translation; }

	void Transform::Rotate(glm::quat rotation) { _rotation += glm::eulerAngles(rotation); }

	void Transform::Rotate(glm::vec3 rotation) { _rotation += rotation; }

	void Transform::Rotate(float x, float y, float z)
	{
		_rotation += glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z));
	}

}