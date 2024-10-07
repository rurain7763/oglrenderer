#include "Texture.h"
#include "../Logger/Logger.h"

Texture::Texture() 
    : _width(0), _height(0)
{
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &_textureID);
}

void Texture::Bind(const int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadFromPng(const char* path) {
    upng_t* png = upng_new_from_file(path);
    if (png == nullptr) {
        LOG_ERR("upng_new_from_file error");
        return;
    }

    upng_decode(png);
    if (upng_get_error(png) != UPNG_EOK) {
        LOG_ERR("upng_decode error");
        return;
    }

    _width = upng_get_width(png);
    _height = upng_get_height(png);
    unsigned int* buffer = new unsigned int[_width * _height * 4];
    memcpy(buffer, upng_get_buffer(png), _width * _height * 4);

    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] buffer;
    upng_free(png);
}