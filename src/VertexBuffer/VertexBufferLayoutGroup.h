#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

struct VertexBufferLayout {
    GLenum type;
    GLuint typeSize;
    GLint count;
    GLboolean normalizedFlag;
};

class VertexBufferLayoutGroup {
public:
    template<typename T>
    void Push(int count);

    template<>
    void Push<float>(int count) {
        VertexBufferLayout entry = {
            .type = GL_FLOAT,
            .typeSize = sizeof(GLfloat),
            .count = static_cast<GLint>(count),
            .normalizedFlag = GL_FALSE
        };
        _entries.push_back(entry);
        _stride += sizeof(GLfloat) * count;
    }

    inline int GetStride() const { return _stride; }
    inline const std::vector<VertexBufferLayout>& GetLayouts() const { return _entries; }

private:
    int _stride = 0;
    std::vector<VertexBufferLayout> _entries;
};

#endif