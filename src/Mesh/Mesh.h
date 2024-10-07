#ifndef MESH_H
#define MESH_H

#include "../Vertex/Vertex.h"
#include "../VertexArray/VertexArray.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayoutGroup.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../Texture/Texture.h"
#include "../Shader/Shader.h"

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh();
    ~Mesh();

    void SetVerticesAndIndices(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    void SetTexture(const std::string& filePath);
    glm::mat4 GetWorldMatrix() const;

    // temporary method
    inline VertexArray* GetVertexArray() const { return _vao; }
    inline IndexBuffer* GetIndexBuffer() const { return _ibo; }

    static Mesh* CreateMeshFromObj(const std::string& filePath);

private:
    VertexArray* _vao;
    VertexBuffer* _vbo;
    IndexBuffer* _ibo;
    Texture* _texture;

    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;
};

#endif