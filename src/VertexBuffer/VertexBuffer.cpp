#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &_vbo);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &_vbo);
}

void VertexBuffer::Init(const void* data, int size) {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}