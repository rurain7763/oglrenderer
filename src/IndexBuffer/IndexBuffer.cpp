#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() 
    : _count(0)
{
    glGenBuffers(1, &_ibo);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &_ibo);
}

void IndexBuffer::Init(const GLuint* data, int count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    _count = count;
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
}   

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}