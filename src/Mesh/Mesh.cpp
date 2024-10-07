#include "Mesh.h"
#include "../Logger/Logger.h"
#include "../Matrix/Matrix.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh() 
    : _vao(nullptr), _vbo(nullptr), _ibo(nullptr)
{
    _vao = new VertexArray();
    _vbo = new VertexBuffer();
    _ibo = new IndexBuffer();

    _position = glm::vec3(0.f, 0.f, 0.f);
    _rotation = glm::vec3(0.f, 0.f, 0.f);
    _scale = glm::vec3(1.f, 1.f, 1.f);
}

Mesh::~Mesh() {
    if(_texture) delete _texture;
    delete _vao;
    delete _vbo;
    delete _ibo;
}

glm::mat4 Mesh::GetWorldMatrix() const{
    glm::mat4 translate = Translation(_position.x, _position.y, _position.z);
    glm::mat4 rotateX = RotationX(_rotation.x);
    glm::mat4 rotateY = RotationY(_rotation.y);
    glm::mat4 rotateZ = RotationZ(_rotation.z);
    glm::mat4 scale = Scaling(_scale.x, _scale.y, _scale.z);

    return translate * rotateX * rotateY * rotateZ * scale;
}

void Mesh::SetVerticesAndIndices(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
    _vao->Bind();
    _vbo->Init(vertices.data(), vertices.size() * sizeof(Vertex));
    _ibo->Init(indices.data(), indices.size());

    VertexBufferLayoutGroup layoutGroup;
    layoutGroup.Push<float>(3);
    layoutGroup.Push<float>(3);
    layoutGroup.Push<float>(2);
    layoutGroup.Push<float>(3);

    _vao->Init(*_vbo, layoutGroup);
}

void Mesh::SetTexture(const std::string& filePath) {
    _vao->Bind();

    _texture = new Texture();
    _texture->LoadFromPng(filePath);
    _texture->Bind(0);
}

Mesh* Mesh::CreateMeshFromObj(const std::string& filePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath, 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices
    );
    if(!scene) {
        LOG_ERR("assimp error: %s", importer.GetErrorString());
        return nullptr;
    }

    Mesh* ret = new Mesh();

    std::vector<Vertex> vertices(scene->mNumMeshes * scene->mMeshes[0]->mNumVertices);
    std::vector<GLuint> indices(scene->mMeshes[0]->mNumFaces * 3);

    for(int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++) {
        const aiMesh* mesh = scene->mMeshes[meshIdx];

        for(int i = 0; i < mesh->mNumVertices; i++) {
            Vertex& vertex = vertices[i];
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.color = glm::vec3(1.f, 1.f, 1.f);
        }

        for(int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];
            for(int j = 0; j < face.mNumIndices; j++) {
                indices[i * 3 + j] = face.mIndices[j];
            }
        }
    }

    ret->SetVerticesAndIndices(vertices, indices);

    importer.FreeScene();

    return ret;
}