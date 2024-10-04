#include "Engine.h"
#include "../Logger/Logger.h"
#include "../Shader/Shader.h"
#include "../VertexArray/VertexArray.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayoutGroup.h"

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

    int windowWidth = 1024;
    int windowHeight = 720;
    _window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL
    );

    _glContext = SDL_GL_CreateContext(_window);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        LOG_ERR("glew init error");
        return;
    }

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
    delete _vbo;
    delete _vao;

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Engine::Setup() {
    _vao = new VertexArray();
    _vbo = new VertexBuffer();
    _shader = new Shader();

    _vao->Bind();

    glm::vec3 vertices[3];
    vertices[0] = glm::vec3(-0.3f, -0.3f, 0.0f); 
    vertices[1] = glm::vec3(0.3f, -0.3f, 0.0f);
    vertices[2] = glm::vec3(0.0f, 0.3f, 0.0f);
    _vbo->Init(vertices, sizeof(vertices));

    VertexBufferLayoutGroup layoutGroup;
    layoutGroup.Push<float>(3);

    _vao->Init(*_vbo, layoutGroup);
    _shader->Init("./assets/shader/shader.vs", "./assets/shader/shader.fs");

    _vao->Unbind();
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
        }
    }
}

void Engine::Update() {
    int timeToWait = (_prevFrameMilliSecs + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - _prevFrameMilliSecs) / 1000.f;
    _prevFrameMilliSecs = SDL_GetTicks();

    _triangleScale += 1.0 * deltaTime;
}

void Engine::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    _vao->Bind();
    _shader->Bind();
    _shader->SetUniform<float>("scale", sin(_triangleScale));
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(_window);
}