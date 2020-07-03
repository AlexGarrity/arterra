#pragma once

#include "PCH.hpp"

#include "window/Window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ecs/component/Transform.hpp"

namespace arterra {

class Camera {
public:
    Camera();

    // Update camera position and view projection matrix
    void Update(Window& window, float_t deltaTime);

    // Get the view projection
    inline glm::mat4 ViewProjection() const { return _viewProjection; }

private:
    float_t _speed = 2.0f;

    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _viewProjection;

    Transform _transform;
};

};