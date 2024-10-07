#include "Camera.h"
#include "../Matrix/Matrix.h"

Camera::Camera() {
    _position = glm::vec3(0.f, 0.f, 0.f);
    _rotation = glm::vec2(0, 0);
    _speed = 10.f;
    _target = glm::vec3(0.f, 0.f, -1.f);
}

glm::mat4 Camera::GetViewMatrix() const {
    return LookAtMatrix(_position, _position + _target, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::SetRotation(const glm::vec2& rotation) {
    _rotation = rotation;

    glm::vec4 front = glm::vec4(0, 0, -1, 0);
    glm::mat4 rotateX = RotationX(_rotation.x);
    glm::mat4 rotateY = RotationY(_rotation.y);

    glm::vec4 newFront = rotateY * rotateX * front;

    _target = glm::vec3(newFront.x, newFront.y, newFront.z);
}