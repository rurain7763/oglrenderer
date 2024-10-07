#include "Engine.h"
#include "../Logger/Logger.h"
#include "../Shader/Shader.h"
#include "../Mesh/Mesh.h"
#include "../Matrix/Matrix.h"
#include "../Vertex/Vertex.h"
#include "../Camera/Camera.h"
#include "../Texture/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Engine::Engine() 
    : _isRunning(false), _prevFrameMilliSecs(0)
{

}

void Engine::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

    _windowWidth = 1024;
    _windowHeight = 720;
    _window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _windowWidth,
        _windowHeight,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL
    );

    _glContext = SDL_GL_CreateContext(_window);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        LOG_ERR("glew init error");
        return;
    }

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    _isRunning = true;
}

void Engine::Run() {
    Setup();
    while(_isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Engine::Destroy() {
    delete _shader;
    delete _mesh;
    delete _camera;

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Engine::Setup() {
    const float aspect = _windowWidth / static_cast<float>(_windowHeight);
    _perspective = Perspective(45.f, aspect, 0.1f, 100.f);

    _camera = new Camera();
    _camera->SetPosition(glm::vec3(0.f, 0.f, 2.f));

    _mesh = Mesh::CreateMeshFromObj("./assets/obj/crab.obj");
    _mesh->SetTexture("./assets/obj/crab.png");

    _shader = new Shader();
    _shader->Init("./assets/shader/shader.vs", "./assets/shader/shader.fs");
    _shader->Bind();
    _shader->SetUniform<int>("texture0", 0);
}

void Engine::ProcessInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                _isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    _isRunning = false;
                }
                if(event.key.keysym.sym == SDLK_w) {
                    _camera->SetPosition(_camera->GetPosition() + _camera->GetFront() * _camera->GetSpeed() * _deltaTime);
                }
                if(event.key.keysym.sym == SDLK_s) {
                    _camera->SetPosition(_camera->GetPosition() - _camera->GetFront() * _camera->GetSpeed() * _deltaTime);
                }
                if(event.key.keysym.sym == SDLK_a) {
                    glm::vec3 right = glm::normalize(glm::cross(_camera->GetFront(), glm::vec3(0.f, 1.f, 0.f)));
                    _camera->SetPosition(_camera->GetPosition() - right * _camera->GetSpeed() * _deltaTime);
                }
                if(event.key.keysym.sym == SDLK_d) {
                    glm::vec3 right = glm::normalize(glm::cross(_camera->GetFront(), glm::vec3(0.f, 1.f, 0.f)));
                    _camera->SetPosition(_camera->GetPosition() + right * _camera->GetSpeed() * _deltaTime);
                }
                if(event.key.keysym.sym == SDLK_LEFT) {
                    _camera->SetRotation(_camera->GetRotation() + glm::vec2(0.0, M_PI * _deltaTime));
                }
                if(event.key.keysym.sym == SDLK_RIGHT) {
                    _camera->SetRotation(_camera->GetRotation() - glm::vec2(0.0, M_PI * _deltaTime));
                }
                if(event.key.keysym.sym == SDLK_UP) {
                    _camera->SetRotation(_camera->GetRotation() + glm::vec2(M_PI * _deltaTime, 0.0));
                }
                if(event.key.keysym.sym == SDLK_DOWN) {
                    _camera->SetRotation(_camera->GetRotation() - glm::vec2(M_PI * _deltaTime, 0.0));
                }
                break;
        }
    }
}

void Engine::Update() {
    int timeToWait = (_prevFrameMilliSecs + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    _deltaTime = (SDL_GetTicks() - _prevFrameMilliSecs) / 1000.f;
    _prevFrameMilliSecs = SDL_GetTicks();
}

void Engine::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMat = _camera->GetViewMatrix();
    glm::mat4 worldMat = _mesh->GetWorldMatrix();

    glm::mat4 mvp = worldMat * viewMat * _perspective;

    _mesh->GetVertexArray()->Bind();
    _mesh->GetIndexBuffer()->Bind();
    _shader->Bind();
    _shader->SetUniform<glm::mat4>("mvp", mvp);
    glDrawElements(GL_TRIANGLES, _mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(_window);
}