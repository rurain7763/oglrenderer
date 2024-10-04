#include "Matrix.h"

glm::mat4 Translation(float x, float y, float z) {
    glm::mat4 ret = IDENTIFY_MAT4();
    ret[0][3] = x;
    ret[1][3] = y;
    ret[2][3] = z;
    return ret;
}

glm::mat4 Scaling(float x, float y, float z) {
    glm::mat4 ret = IDENTIFY_MAT4();
    ret[0][0] = x;
    ret[1][1] = y;
    ret[2][2] = z;
    return ret;
}