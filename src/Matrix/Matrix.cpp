#include "Matrix.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Translation(float x, float y, float z) {
    glm::mat4 ret = IDENTIFY_MAT4();
    ret[0][3] = x;
    ret[1][3] = y;
    ret[2][3] = z;
    return ret;
}

glm::mat4 RotationX(float rot) {
    float s = sin(rot);
    float c = cos(rot);

    glm::mat4 ret = IDENTIFY_MAT4();
    ret[1][1] = c;
    ret[1][2] = s;
    ret[2][1] = -s;
    ret[2][2] = c;

    return ret;
}

glm::mat4 RotationY(float rot) {
    float s = sin(rot);
    float c = cos(rot);

    glm::mat4 ret = IDENTIFY_MAT4();
    ret[0][0] = c;
    ret[0][2] = -s;
    ret[2][0] = s;
    ret[2][2] = c;

    return ret;
}

glm::mat4 RotationZ(float rot) {
    float s = sin(rot);
    float c = cos(rot);

    glm::mat4 ret = IDENTIFY_MAT4();
    ret[0][0] = c;
    ret[0][1] = s;
    ret[1][0] = -s;
    ret[1][1] = c;
    
    return ret;
}

glm::mat4 Scaling(float x, float y, float z) {
    glm::mat4 ret = IDENTIFY_MAT4();
    ret[0][0] = x;
    ret[1][1] = y;
    ret[2][2] = z;
    return ret;
}

glm::mat4 WorldMatrix(const glm::mat4& translate, const glm::mat4& rotate, const glm::mat4& scale) {
    return scale * rotate * translate;
}

glm::mat4 Perspective(float fov, float aspect, float near, float far) {
    glm::mat4 ret = glm::mat4(0.0f);
    float tanHalfFov = tan(fov / 2);
    ret[0][0] = 1.0f / (aspect * tanHalfFov);   
    ret[1][1] = 1.0f / tanHalfFov;              
    ret[2][2] = -(far + near) / (far - near);  
    ret[2][3] = -(2.0f * far * near) / (far - near);
    ret[3][2] = -1.0f; // opengl uses right handed coordinate system so we need to negate this value           
    return ret;
}


glm::mat4 LookAtMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
    glm::vec3 zAxis = glm::normalize(position - target);
    glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis)); 
    glm::vec3 yAxis = glm::cross(zAxis, xAxis);
    
    glm::mat4 ret = {
        xAxis.x, xAxis.y, xAxis.z, -glm::dot(xAxis, position),
        yAxis.x, yAxis.y, yAxis.z, -glm::dot(yAxis, position),
        zAxis.x, zAxis.y, zAxis.z, -glm::dot(zAxis, position),
        0, 0, 0, 1
    };

    return ret;
}
