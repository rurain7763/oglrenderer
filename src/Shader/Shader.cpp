#include "Shader.h"
#include "../Logger/Logger.h"

#include <fstream>

Shader::Shader() 
    : _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{

}

Shader::~Shader() {
    glDeleteProgram(_programID);
    glDeleteShader(_vertexShaderID);
    glDeleteShader(_fragmentShaderID);
}

void Shader::Init(const std::string& vsFilePath, const std::string& fsFilePath) {
    _programID = glCreateProgram();

    if (_programID == 0) {
        LOG_ERR("error creating shader program");
        return;
    }

    std::string vs, fs;

    if (!ReadFile(vsFilePath, vs)) {
        LOG_ERR("vertex shader path not valid");
        return;
    };

    if(!AttachShader(vs, GL_VERTEX_SHADER)) {
        LOG_ERR("vertex shader attach failed");
        return;
    }

    if(!ReadFile(fsFilePath, fs)) {
        LOG_ERR("fragment shader path not valid");
        return;
    };

    if(!AttachShader(fs, GL_FRAGMENT_SHADER)) {
        LOG_ERR("fragment shader attach failed");
        return;
    }

    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(_programID);

    glGetProgramiv(_programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programID, sizeof(errorLog), NULL, errorLog);
        LOG_ERR( "error linking shader program: '%s'", errorLog);
        return;
    }

    glValidateProgram(_programID);
    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programID, sizeof(errorLog), NULL, errorLog);
        LOG_ERR("invalid shader program: '%s'", errorLog);
        return;
    }
}

void Shader::Bind() const {
    glUseProgram(_programID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

bool Shader::ReadFile(const std::string& filePath, std::string& out) {
    std::ifstream file(filePath);
    if(!file.is_open()) return false;
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    out.resize(size);
    file.read(&out[0], size);
    return true;
}

bool Shader::AttachShader(const std::string& shaderText, GLenum shaderType) {
    GLuint& shaderID = shaderType == GL_VERTEX_SHADER ? _vertexShaderID : _fragmentShaderID;
    shaderID = glCreateShader(shaderType);

    if (shaderID == 0) {
        LOG_ERR("error creating shader type %d", shaderType);
        return false;
    }

    const GLchar* p[] = { shaderText.c_str() };
    const GLint Lengths[] = { (GLint)shaderText.size() };

    glShaderSource(shaderID, 1, p, Lengths);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
        LOG_ERR("Error compiling shader type %d: '%s'\n", shaderType, infoLog);
        return false;
    }

    glAttachShader(_programID, shaderID);

    return true;
}

int Shader::GetUniformLocation(const std::string& name) {
    if(_uniformLocCache.find(name) != _uniformLocCache.end()) {
        return _uniformLocCache[name];
    }

    int location = glGetUniformLocation(_programID, name.c_str());
    if (location == -1) {
        LOG_WARN("uniform name '%s' doesn't exist!", name.c_str());
    } 

    _uniformLocCache[name] = location;
    return location;
}
