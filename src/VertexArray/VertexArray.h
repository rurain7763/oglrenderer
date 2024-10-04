#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include <GL/freeglut.h>

class VertexBuffer;
class VertexBufferLayoutGroup;

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Init(const VertexBuffer& vbo, const VertexBufferLayoutGroup& layoutGroup);

    void Bind() const;
    void Unbind() const;

private:
    GLuint _vao;
};

#endif