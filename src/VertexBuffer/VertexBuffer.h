#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GL/glew.h>
#include <GL/freeglut.h>

class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();

    void Init(const void* data, int size);

    void Bind() const;
    void Unbind() const;

private:
    GLuint _vbo;
};

#endif
