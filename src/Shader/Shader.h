#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader {
public:
    Shader();
    ~Shader();

    void Init(const std::string& vsFilePath, const std::string& fsFilePath);
    
    void Bind() const;
    void Unbind() const;

    template<typename... Args>
    void SetUniform(const std::string& name, const Args&... args) {}

private:
    bool ReadFile(const std::string& filePath, std::string& out);
    bool AttachShader(const std::string& shaderText, GLenum shaderType);

    int GetUniformLocation(const std::string& name);

    template<>
    void SetUniform<int>(const std::string& name, const int& v) {
        glUniform1i(GetUniformLocation(name), v);
    }

    template<>
    void SetUniform<float>(const std::string& name, const float& v) {
        glUniform1f(GetUniformLocation(name), v);
    }

    template<>
    void SetUniform<glm::mat4>(const std::string& name, const glm::mat4& v) {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &v[0][0]);
    }

private:
    GLuint _programID;
    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;

    std::unordered_map<std::string, int> _uniformLocCache;
};

#endif