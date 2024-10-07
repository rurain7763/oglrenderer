#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    ~Camera() = default;

    glm::mat4 GetViewMatrix() const;

    inline void SetPosition(const glm::vec3& position) { _position = position; };
    inline glm::vec3 GetPosition() const { return _position; };

    void SetRotation(const glm::vec2& rotation);
    inline glm::vec2 GetRotation() const { return _rotation; };

    inline void SetSpeed(float speed) { _speed = speed; };
    inline float GetSpeed() const { return _speed; };
    
    inline glm::vec3 GetFront() const { return _target; };

private:
    glm::vec3 _position;
    glm::vec2 _rotation;

    float _speed;
    glm::vec3 _target;
};

#endif