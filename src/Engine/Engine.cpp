#include "Engine.h"
#include "../Logger/Logger.h"
#include "../Shader/Shader.h"
#include "../VertexArray/VertexArray.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayoutGroup.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../Matrix/Matrix.h"
#include "../Vertex/Vertex.h"

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
    delete _shader;
    delete _ibo;
    delete _vbo;
    delete _vao;

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Engine::Setup() {
    _vao = new VertexArray();
    _vbo = new VertexBuffer();
    _ibo = new IndexBuffer();
    _shader = new Shader();

    _vao->Bind();

    Vertex vertices[] = {
        { glm::vec3(-0.3f, -0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(-0.3f, 0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, -0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(-0.3f, -0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(-0.3f, 0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, 0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, -0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) }
    };

    _vbo->Init(vertices, sizeof(vertices));

#if false
    GLuint indices[] = { 
        0, 1, 2, 
        0, 2, 3, 
        1, 5, 6, 
        1, 6, 2, 
        5, 4, 7, 
        5, 7, 6, 
        4, 0, 3, 
        4, 3, 7, 
        3, 2, 6, 
        3, 6, 7, 
        0, 4, 5, 
        0, 5, 1 
    };

    _ibo->Init(indices, 12);
#else
    GLuint indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    _ibo->Init(indices, 24);
#endif


    VertexBufferLayoutGroup layoutGroup;
    layoutGroup.Push<float>(3);
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
}

void Engine::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 translate = Translation(0, 0, 0);
    glm::mat4 rotateX = RotationX(SDL_GetTicks() / 1000.f);
    glm::mat4 rotateY = RotationY(SDL_GetTicks() / 1000.f);
    glm::mat4 rotateZ = RotationZ(0);
    glm::mat4 scale = Scaling(1, 1, 1);
    glm::mat4 worldMat = WorldMatrix(translate, rotateX * rotateY * rotateZ, scale);

    _vao->Bind();
    _shader->Bind();
    _shader->SetUniform<glm::mat4>("world", worldMat);
    _ibo->Bind();
    //glDrawElements(GL_TRIANGLES, _ibo->GetCount(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINES, _ibo->GetCount(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(_window);
}