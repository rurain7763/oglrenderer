#include "Engine.h"
#include "../Logger/Logger.h"
#include "../Shader/Shader.h"
#include "../VertexArray/VertexArray.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayoutGroup.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../Matrix/Matrix.h"
#include "../Vertex/Vertex.h"
#include "../Camera/Camera.h"
#include "../Texture/Texture.h"

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
    delete _texture;
    delete _shader;
    delete _ibo;
    delete _vbo;
    delete _vao;

    delete _camera;

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Engine::Setup() {
    const float aspect = _windowWidth / static_cast<float>(_windowHeight);

    _camera = new Camera();
    _camera->SetPosition(glm::vec3(0.f, 0.f, 0.f));

    _perspective = Perspective(45.f, aspect, 0.1f, 100.f);

    _vao = new VertexArray();
    _vbo = new VertexBuffer();
    _ibo = new IndexBuffer();
    _shader = new Shader();

    _vao->Bind();

    Vertex vertices[] = {
        { glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0, 0), glm::vec3(1, 1, 1) },
        { glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0, 1), glm::vec3(1, 1, 1) },
        { glm::vec3(0.5, 0.5, 0.5), glm::vec2(1, 1), glm::vec3(1, 1, 1) },
        { glm::vec3(0.5, -0.5, 0.5), glm::vec2(1, 0), glm::vec3(1, 1, 1) },
    };

    GLuint indices[] = { 
        0, 1, 2,
        2, 3, 0,
    };

    _vbo->Init(vertices, sizeof(vertices));
    _ibo->Init(indices, sizeof(indices) / sizeof(GLuint));

    VertexBufferLayoutGroup layoutGroup;
    layoutGroup.Push<float>(3);
    layoutGroup.Push<float>(2);
    layoutGroup.Push<float>(3);

    _vao->Init(*_vbo, layoutGroup);

    _texture = new Texture();
    _texture->LoadFromPng("./assets/images/cube.png");
    _texture->Bind(0);

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
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 viewMat = _camera->GetViewMatrix();

    float time = SDL_GetTicks() / 1000.f;
    glm::mat4 translate = Translation(0, 0, -1.5);
    glm::mat4 rotateX = RotationX(time * 0);
    glm::mat4 rotateY = RotationY(time * 0);
    glm::mat4 rotateZ = RotationZ(0);
    glm::mat4 scale = Scaling(1, 1, 1);
    glm::mat4 worldMat = WorldMatrix(translate, rotateX * rotateY * rotateZ, scale);

    glm::mat4 mvp = worldMat * viewMat * _perspective;

    _vao->Bind();
    _shader->Bind();
    _shader->SetUniform<glm::mat4>("mvp", mvp);
    _ibo->Bind();
    glDrawElements(GL_TRIANGLES, _ibo->GetCount(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(_window);
}