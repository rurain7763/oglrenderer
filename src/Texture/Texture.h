#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../vendor/upng/upng.h"

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Texture {
public:
    Texture();
    ~Texture();

    void Bind(const int slot) const;
    void Unbind() const;

    void LoadFromPng(const std::string& path);

private:
    GLuint _textureID;
    int _width, _height;
};

#endif