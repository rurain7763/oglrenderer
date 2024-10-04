#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Shader {
public:
    Shader();
    ~Shader();

    void Init(const std::string& vsFilePath, const std::string& fsFilePath);
    
    void Bind();

private:
    bool ReadFile(const std::string& filePath, std::string& out);
    bool AttachShader(const std::string& shaderText, GLenum shaderType);

private:
    GLuint _programID;
    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;
};

#endif