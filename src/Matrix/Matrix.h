#ifndef MATRIX_H
#define MATRIX_H

#include <glm/glm.hpp>

#define IDENTIFY_MAT4() glm::mat4( \
                            1, 0, 0, 0, \
                            0, 1, 0, 0, \
                            0, 0, 1, 0, \
                            0, 0, 0, 1 \
                        );

glm::mat4 Translation(float x, float y, float z);
glm::mat4 RotationZ(float rot);
glm::mat4 Scaling(float x, float y, float z);

#endif