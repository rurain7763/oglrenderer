#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <GL/glew.h>
#include <GL/freeglut.h>

class IndexBuffer {
public:
    IndexBuffer();
    ~IndexBuffer();

    void Init(const GLuint* data, int count);

    void Bind() const;
    void Unbind() const; 

    inline int GetCount() const { return _count; }

private:
    GLuint _ibo;
    int _count;
};

#endif